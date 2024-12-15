#include "Renderer.hpp"
#include <algorithm>
#include "Scene.hpp"
#include "../objects/Object3D.hpp"
#include "../../utils/math/Matrix4.hpp"
#include "../../utils/math/Vector3.hpp"
#include "../../utils/math/Vector4.hpp"
#include <memory>
#include "../../utils/WindowManager.hpp"

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
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    if (!scene.getVerticesEditMode()) {
        renderFaces(facesToRender);
    }
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
    int i = 0;
    for (const auto& object : objects) {
        if (!i) {
            i++;
            continue;
        }
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        if (!object->isSelected && !scene.getVerticesEditMode()) continue;

        Matrix4 modelMatrix = object->getTransformation();
        Matrix4 viewMatrix = camera.getViewMatrix();
        Matrix4 projectionMatrix = camera.getProjectionMatrix();

        std::vector<Vector3> transformedVertices;
        for (const auto& vertex : object->getVertices()) {
            Vector4 worldVertex = modelMatrix * Vector4(vertex, 1.0f);
            Vector4 cameraVertex = viewMatrix * worldVertex;
            transformedVertices.push_back(Vector3(cameraVertex.x, cameraVertex.y, cameraVertex.z));
        }

        if (!isObjectInFrustum(transformedVertices)) {
            continue;
        }

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

            sf::Vector2f screenPos1 = {
                (v1.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - v1.y) * 0.5f * window.getSize().y
            };
            sf::Vector2f screenPos2 = {
                (v2.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - v2.y) * 0.5f * window.getSize().y
            };

            sf::Color color = sf::Color(255, 255, 255, 64);
            if (scene.getVerticesEditMode()) {
                color = sf::Color::White;
                if (object->isSelected) {
                    color = sf::Color::Yellow;
                }
            }

            sf::Vertex edgeVertices[] = {
                sf::Vertex(screenPos1, color),
                sf::Vertex(screenPos2, color)
            };

            window.draw(edgeVertices, 2, sf::Lines);
        }
        if (scene.getVerticesEditMode()) {
            int j = 0;
            for (const auto& vertex : projectedVertices) {
                if (vertex.z < 0.1f || vertex.z > 100.0f) continue;

                sf::Vector2f screenPos = {
                    (vertex.x + 1.0f) * 0.5f * window.getSize().x,
                    (1.0f - vertex.y) * 0.5f * window.getSize().y
                };
                sf::Color vertexColor = object->isVertexSelected(j) ? sf::Color::Yellow : sf::Color(150, 150, 150, 230);
                sf::CircleShape vertexCircle(10.0f);
                vertexCircle.setFillColor(vertexColor);
                vertexCircle.setPosition(screenPos.x - vertexCircle.getRadius(),
                                         screenPos.y - vertexCircle.getRadius());

                window.draw(vertexCircle);
                j++;
            }
        }
    }
}