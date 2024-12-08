#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Camera.hpp"
#include "../objects/Object3D.hpp"
#include "../objects/Cube.hpp"
#include "../../utils/Matrix4.hpp"
#include "../../utils/Vector3.hpp"
#include "../../utils/Vector4.hpp"
#include <iostream>

struct FaceData {
    Vector3 v1, v2, v3;
    sf::Color color;
    float depth;
    std::shared_ptr<Object3d> object;
    int groupID;
};

class Renderer {
public:
    Renderer(sf::RenderWindow& window) : window(window) {}

    void render(std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera) {
    std::vector<FaceData> facesToRender;
    for (auto& object : objects) {
        Matrix4 modelMatrix = object->getTransformation();
        Matrix4 viewMatrix = camera.getViewMatrix();
        Matrix4 projectionMatrix = camera.getProjectionMatrix();

        // First, check if the object is behind the camera
        Vector3 objectPosition = object->getPosition(); // Assuming you have a getPosition() method
        Vector4 worldPosition = modelMatrix * Vector4(objectPosition, 1.0f);
        Vector4 cameraPosition = viewMatrix * worldPosition;

        if (cameraPosition.z < 0.0f) {
            continue; // Skip object if it's behind the camera
        }

        // Proceed with processing the object if it's not behind the camera
        std::vector<Vector3> cameraSpaceVertices;
        for (const auto& vertex : object->getVertices()) {
            Vector4 worldVertex = modelMatrix * Vector4(vertex, 1.0f);
            Vector4 cameraVertex = viewMatrix * worldVertex;
            cameraSpaceVertices.push_back(Vector3(cameraVertex.x, cameraVertex.y, cameraVertex.z));
        }

        std::vector<Vector3> projectedVertices;
        for (const auto& vertex : cameraSpaceVertices) {
            Vector4 projectedVertex = projectionMatrix * Vector4(vertex, 1.0f);

            if (projectedVertex.w != 0.0f) {
                projectedVertex.x /= projectedVertex.w;
                projectedVertex.y /= projectedVertex.w;
                projectedVertex.z /= projectedVertex.w;
            }

            projectedVertices.push_back(Vector3(projectedVertex.x, projectedVertex.y, projectedVertex.z));
        }

        auto sortedFaces = object->getSortedFaces(cameraSpaceVertices);

        for (size_t i = 0; i < sortedFaces.size(); i += 2) {
            if (i + 1 >= sortedFaces.size()) break;

            const auto& triangle1 = sortedFaces[i];
            const auto& triangle2 = sortedFaces[i + 1];

            float zAvg = (
                cameraSpaceVertices[triangle1[0]].z +
                cameraSpaceVertices[triangle1[1]].z +
                cameraSpaceVertices[triangle1[2]].z +
                cameraSpaceVertices[triangle2[0]].z +
                cameraSpaceVertices[triangle2[1]].z +
                cameraSpaceVertices[triangle2[2]].z
            ) / 6.0f;

            FaceData faceData1, faceData2;
            faceData1.v1 = projectedVertices[triangle1[0]];
            faceData1.v2 = projectedVertices[triangle1[1]];
            faceData1.v3 = projectedVertices[triangle1[2]];
            faceData1.color = sf::Color(triangle1[3], triangle1[4], triangle1[5]);
            faceData1.depth = zAvg;
            faceData1.object = object;
            faceData1.groupID = static_cast<int>(i / 2);

            faceData2.v1 = projectedVertices[triangle2[0]];
            faceData2.v2 = projectedVertices[triangle2[1]];
            faceData2.v3 = projectedVertices[triangle2[2]];
            faceData2.color = sf::Color(triangle2[3], triangle2[4], triangle2[5]);
            faceData2.depth = zAvg;
            faceData2.object = object;
            faceData2.groupID = static_cast<int>(i / 2);

            facesToRender.push_back(faceData1);
            facesToRender.push_back(faceData2);
        }
    }

    std::stable_sort(facesToRender.begin(), facesToRender.end(), [](const FaceData& a, const FaceData& b) {
        if (a.depth != b.depth)
            return a.depth > b.depth;

        if (a.groupID == b.groupID)
            return a.object < b.object;

        return a.groupID < b.groupID;
    });

    renderFaces(facesToRender);
    renderEdges(objects, camera);
}


private:
    sf::RenderWindow& window;

    void renderFaces(const std::vector<FaceData>& facesToRender) {
        for (const auto& face : facesToRender) {
            // Compute the average depth (center point) of the face
            if (face.v1.z < 0.1f || face.v2.z < 0.1f || face.v3.z < 0.1f ||
            face.v1.z > 100.0f || face.v2.z > 100.0f || face.v3.z > 100.0f) {
                continue; // Skip face if it’s out of bounds
            }

            // Convert 3D vertices to 2D screen space for rendering
            sf::Vector2f screenPos1 = {
                (face.v1.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - face.v1.y) * 0.5f * window.getSize().y
            };
            sf::Vector2f screenPos2 = {
                (face.v2.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - face.v2.y) * 0.5f * window.getSize().y
            };
            sf::Vector2f screenPos3 = {
                (face.v3.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - face.v3.y) * 0.5f * window.getSize().y
            };

            // Draw the triangle representing the face
            sf::Vertex triangle[] = {
                sf::Vertex(screenPos1, face.color),
                sf::Vertex(screenPos2, face.color),
                sf::Vertex(screenPos3, face.color)
            };

            window.draw(triangle, 3, sf::Triangles);
        }
    }


    void renderEdges(std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera) {
    for (const auto& object : objects) {
        if (!object->isSelected) continue;

        Matrix4 modelMatrix = object->getTransformation();
        Matrix4 viewMatrix = camera.getViewMatrix();
        Matrix4 projectionMatrix = camera.getProjectionMatrix();

        // Transform vertices into camera space
        std::vector<Vector3> transformedVertices;
        for (const auto& vertex : object->getVertices()) {
            Vector4 worldVertex = modelMatrix * Vector4(vertex, 1.0f);
            Vector4 cameraVertex = viewMatrix * worldVertex;
            transformedVertices.push_back(Vector3(cameraVertex.x, cameraVertex.y, cameraVertex.z));
        }

        // Frustum culling: check if any vertex is within the near and far plane
        bool isInsideFrustum = false;
        for (const auto& v : transformedVertices) {
            if (v.z >= 0.1f && v.z < 100.0f) {
                isInsideFrustum = true;
                break;
            }
        }

        if (!isInsideFrustum) {
            continue;  // Skip rendering if object is not inside frustum
        }

        // Project vertices to 2D screen space
        std::vector<Vector3> projectedVertices;
        for (auto& vertex : transformedVertices) {
            Vector4 projectedVertex = projectionMatrix * Vector4(vertex, 1.0f);

            if (projectedVertex.w != 0.0f) {
                projectedVertex.x /= projectedVertex.w;
                projectedVertex.y /= projectedVertex.w;
                projectedVertex.z /= projectedVertex.w;
            }

            projectedVertices.push_back(Vector3(projectedVertex.x, projectedVertex.y, projectedVertex.z));
        }

        // Render edges with frustum culling
        auto edges = object->getEdges();
        for (const auto& edge : edges) {
            int idx1 = edge.first;
            int idx2 = edge.second;

            const Vector3& v1 = projectedVertices[idx1];
            const Vector3& v2 = projectedVertices[idx2];

            if (v1.z < 0.1f || v2.z < 0.1f || v1.z > 100.0f || v2.z > 100.0f)
                continue;

            if (v1.x < -1.0f || v1.x > 1.0f || v1.y < -1.0f || v1.y > 1.0f ||
                v2.x < -1.0f || v2.x > 1.0f || v2.y < -1.0f || v2.y > 1.0f)
                continue;

            // Convert to screen space
            sf::Vector2f screenPos1 = {
                (v1.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - v1.y) * 0.5f * window.getSize().y
            };
            sf::Vector2f screenPos2 = {
                (v2.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - v2.y) * 0.5f * window.getSize().y
            };

            sf::Color transparentWhite(255, 255, 255, 64);

            sf::Vertex edgeVertices[] = {
                sf::Vertex(screenPos1, transparentWhite),
                sf::Vertex(screenPos2, transparentWhite)
            };

            window.draw(edgeVertices, 2, sf::Lines);
        }
    }

}



};

#endif
