#include "Renderer.hpp"
#include <algorithm>
#include "../objects/Object3D.hpp"
#include "../objects/Cube.hpp"
#include "../../utils/math/Matrix4.hpp"
#include "../../utils/math/Vector3.hpp"
#include "../../utils/math/Vector4.hpp"
#include <memory>
#include <cmath>

Renderer::Renderer(sf::RenderWindow& window) : window(window) {}

bool Renderer::isFaceCulled(const Vector3& v1, const Vector3& v2, const Vector3& v3) {
    const float nearPlane = 0.1f;
    const float farPlane = 100.0f;

    return (v1.z < nearPlane || v1.z > farPlane) &&
           (v2.z < nearPlane || v2.z > farPlane) &&
           (v3.z < nearPlane || v3.z > farPlane);
}

bool Renderer::isObjectInFrustum(const std::vector<Vector3>& vertices) {
    const float nearPlane = 0.1f;
    const float farPlane = 100.0f;

    for (const auto& v : vertices) {
        if (v.z >= nearPlane && v.z <= farPlane) {
            return true;  // Object is within frustum
        }
    }
    return false;
}

void Renderer::render(std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera) {
    std::vector<FaceData> facesToRender;

    for (auto& object : objects) {
        Matrix4 modelMatrix = object->getTransformation();
        Matrix4 viewMatrix = camera.getViewMatrix();
        Matrix4 projectionMatrix = camera.getProjectionMatrix();

        Vector3 objectPosition = object->getPosition();
        Vector4 worldPosition = modelMatrix * Vector4(objectPosition, 1.0f);
        Vector4 cameraPosition = viewMatrix * worldPosition;

        if (cameraPosition.z < 0.0f) {
            continue;
        }

        std::vector<Vector3> cameraSpaceVertices;
        for (const auto& vertex : object->getVertices()) {
            Vector4 worldVertex = modelMatrix * Vector4(vertex, 1.0f);
            Vector4 cameraVertex = viewMatrix * worldVertex;
            cameraSpaceVertices.push_back(Vector3(cameraVertex.x, cameraVertex.y, cameraVertex.z));
        }

        if (!isObjectInFrustum(cameraSpaceVertices)) {
            continue;
        }

        // Project vertices
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

        for (const auto& triangle : sortedFaces) {
            float zAvg = (
                cameraSpaceVertices[triangle[0]].z +
                cameraSpaceVertices[triangle[1]].z +
                cameraSpaceVertices[triangle[2]].z
            ) / 3.0f;

            if (isFaceCulled(projectedVertices[triangle[0]],
                             projectedVertices[triangle[1]],
                             projectedVertices[triangle[2]])) {
                continue;
            }

            FaceData faceData;
            faceData.v1 = projectedVertices[triangle[0]];
            faceData.v2 = projectedVertices[triangle[1]];
            faceData.v3 = projectedVertices[triangle[2]];
            faceData.color = sf::Color(triangle[3], triangle[4], triangle[5], triangle[6]);
            faceData.depth = zAvg;
            faceData.object = object;

            facesToRender.push_back(faceData);
        }
    }

    std::stable_sort(facesToRender.begin(), facesToRender.end(),
        [](const FaceData& a, const FaceData& b) {
            if (a.depth != b.depth)
                return a.depth > b.depth;
            return a.object < b.object;
        }
    );

    renderFaces(facesToRender);
    renderEdges(objects, camera);
}

void Renderer::renderFaces(const std::vector<FaceData>& facesToRender) {
    for (const auto& face : facesToRender) {
        if (face.v1.z < 0.1f || face.v2.z < 0.1f || face.v3.z < 0.1f ||
            face.v1.z > 100.0f || face.v2.z > 100.0f || face.v3.z > 100.0f) {
            continue;
        }

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

        sf::Vertex triangle[] = {
            sf::Vertex(screenPos1, face.color),
            sf::Vertex(screenPos2, face.color),
            sf::Vertex(screenPos3, face.color)
        };

        window.draw(triangle, 3, sf::Triangles);
    }
}

void Renderer::renderEdges(std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera) {
    for (const auto& object : objects) {
        // Only render edges for selected objects
        if (!object->isSelected) continue;

        Matrix4 modelMatrix = object->getTransformation();
        Matrix4 viewMatrix = camera.getViewMatrix();
        Matrix4 projectionMatrix = camera.getProjectionMatrix();

        // Transform vertices to camera space
        std::vector<Vector3> transformedVertices;
        for (const auto& vertex : object->getVertices()) {
            Vector4 worldVertex = modelMatrix * Vector4(vertex, 1.0f);
            Vector4 cameraVertex = viewMatrix * worldVertex;
            transformedVertices.push_back(Vector3(cameraVertex.x, cameraVertex.y, cameraVertex.z));
        }

        // Skip rendering if object is completely outside frustum
        if (!isObjectInFrustum(transformedVertices)) {
            continue;
        }

        // Project vertices
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

        // Render edges
        auto edges = object->getEdges();
        for (const auto& edge : edges) {
            int idx1 = edge.first;
            int idx2 = edge.second;

            const Vector3& v1 = projectedVertices[idx1];
            const Vector3& v2 = projectedVertices[idx2];

            // Skip edges outside frustum or clipping planes
            if (v1.z < 0.1f || v2.z < 0.1f || v1.z > 100.0f || v2.z > 100.0f)
                continue;

            // Skip edges outside screen
            if (v1.x < -1.0f || v1.x > 1.0f || v1.y < -1.0f || v1.y > 1.0f ||
                v2.x < -1.0f || v2.x > 1.0f || v2.y < -1.0f || v2.y > 1.0f)
                continue;

            // Convert to screen coordinates
            sf::Vector2f screenPos1 = {
                (v1.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - v1.y) * 0.5f * window.getSize().y
            };
            sf::Vector2f screenPos2 = {
                (v2.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - v2.y) * 0.5f * window.getSize().y
            };

            // Slightly transparent white for edge color
            sf::Color transparentWhite(255, 255, 255, 64);

            sf::Vertex edgeVertices[] = {
                sf::Vertex(screenPos1, transparentWhite),
                sf::Vertex(screenPos2, transparentWhite)
            };

            window.draw(edgeVertices, 2, sf::Lines);
        }
    }
}