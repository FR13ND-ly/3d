#include "Renderer.hpp"
#include <algorithm>
#include "Scene.hpp"
#include "../../utils/WindowManager.hpp"

constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE = 100.0f;

Renderer::Renderer(sf::RenderWindow& window) : window(window) {}

bool Renderer::isFaceCulled(const Vector3& v1, const Vector3& v2, const Vector3& v3) const {
    return (v1.z < NEAR_PLANE || v1.z > FAR_PLANE) &&
           (v2.z < NEAR_PLANE || v2.z > FAR_PLANE) &&
           (v3.z < NEAR_PLANE || v3.z > FAR_PLANE);
}

bool Renderer::isObjectInFrustum(const std::vector<Vector3>& vertices) const {
    return std::any_of(vertices.begin(), vertices.end(), [](const Vector3& v) {
        return v.z >= NEAR_PLANE && v.z <= FAR_PLANE;
    });
}

std::vector<Vector3> Renderer::transformVertices(const std::vector<Vector3>& vertices, const Matrix4& modelMatrix, const Matrix4& viewMatrix) const {
    std::vector<Vector3> transformedVertices;
    transformedVertices.reserve(vertices.size());

    for (const auto& vertex : vertices) {
        Vector4 worldVertex = modelMatrix * Vector4(vertex, 1.0f);
        Vector4 cameraVertex = viewMatrix * worldVertex;
        transformedVertices.emplace_back(cameraVertex.x, cameraVertex.y, cameraVertex.z);
    }

    return transformedVertices;
}

std::vector<Vector3> Renderer::projectVertices(const std::vector<Vector3>& vertices, const Matrix4& projectionMatrix) const {
    std::vector<Vector3> projectedVertices;
    projectedVertices.reserve(vertices.size());

    for (const auto& vertex : vertices) {
        Vector4 projectedVertex = projectionMatrix * Vector4(vertex, 1.0f);
        if (projectedVertex.w != 0.0f) {
            projectedVertex.x /= projectedVertex.w;
            projectedVertex.y /= projectedVertex.w;
            projectedVertex.z /= projectedVertex.w;
        }
        projectedVertices.emplace_back(projectedVertex.x, projectedVertex.y, projectedVertex.z);
    }

    return projectedVertices;
}

sf::Vector2f Renderer::screenPosition(const Vector3& vertex) const {
    return {
        (vertex.x + 1.0f) * 0.5f * window.getSize().x,
        (1.0f - vertex.y) * 0.5f * window.getSize().y
    };
}

void Renderer::render(const std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera) {
    std::vector<FaceData> facesToRender;

    for (const auto& object : objects) {
        processObject(object, camera, facesToRender);
    }

    std::stable_sort(facesToRender.begin(), facesToRender.end(), [](const FaceData& a, const FaceData& b) {
        if (a.depth != b.depth) return a.depth > b.depth;
        return a.object < b.object;
    });

    renderFaces(facesToRender);
    renderEdges(objects, camera);
}

void Renderer::processObject(const std::shared_ptr<Object3d>& object, Camera& camera, std::vector<FaceData>& facesToRender) {
    Matrix4 modelMatrix = object->getTransformation();
    Matrix4 viewMatrix = camera.getViewMatrix();
    Matrix4 projectionMatrix = camera.getProjectionMatrix();

    Vector3 objectPosition = object->getPosition();
    Vector4 worldPosition = modelMatrix * Vector4(objectPosition, 1.0f);
    Vector4 cameraPosition = viewMatrix * worldPosition;

    if (cameraPosition.z < 0.0f) return;

    std::vector<Vector3> cameraSpaceVertices = transformVertices(object->getVertices(), modelMatrix, viewMatrix);

    if (!isObjectInFrustum(cameraSpaceVertices)) return;

    std::vector<Vector3> projectedVertices = projectVertices(cameraSpaceVertices, projectionMatrix);

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

        facesToRender.push_back({
            projectedVertices[triangle[0]],
            projectedVertices[triangle[1]],
            projectedVertices[triangle[2]],
            sf::Color(triangle[3], triangle[4], triangle[5], triangle[6]),
            zAvg,
            object
        });
    }
}

void Renderer::renderFaces(const std::vector<FaceData>& facesToRender) {
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);

    for (const auto& face : facesToRender) {
        if (face.v1.z < NEAR_PLANE || face.v2.z < NEAR_PLANE || face.v3.z < NEAR_PLANE ||
            face.v1.z > FAR_PLANE || face.v2.z > FAR_PLANE || face.v3.z > FAR_PLANE) {
            continue;
        }

        sf::Vector2f screenPos1 = screenPosition(face.v1);
        sf::Vector2f screenPos2 = screenPosition(face.v2);
        sf::Vector2f screenPos3 = screenPosition(face.v3);

        sf::Color color = scene.getVerticesEditMode() ? sf::Color(255, 255, 255, 32) : face.color;

        sf::Vertex triangle[] = {
            sf::Vertex(screenPos1, color),
            sf::Vertex(screenPos2, color),
            sf::Vertex(screenPos3, color)
        };

        window.draw(triangle, 3, sf::Triangles);
    }
}

void Renderer::renderEdges(const std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera) {
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);

    for (const auto& object : objects) {
        if (!object->isSelected && !scene.getVerticesEditMode()) continue;

        Matrix4 modelMatrix = object->getTransformation();
        Matrix4 viewMatrix = camera.getViewMatrix();
        Matrix4 projectionMatrix = camera.getProjectionMatrix();

        std::vector<Vector3> transformedVertices = transformVertices(object->getVertices(), modelMatrix, viewMatrix);

        if (!isObjectInFrustum(transformedVertices)) continue;

        std::vector<Vector3> projectedVertices = projectVertices(transformedVertices, projectionMatrix);

        auto edges = object->getEdges();
        for (const auto& edge : edges) {
            int idx1 = edge.first;
            int idx2 = edge.second;

            const Vector3& v1 = projectedVertices[idx1];
            const Vector3& v2 = projectedVertices[idx2];

            if (v1.z < NEAR_PLANE || v2.z < NEAR_PLANE || v1.z > FAR_PLANE || v2.z > FAR_PLANE) continue;
            if (v1.x < -1.0f || v1.x > 1.0f || v1.y < -1.0f || v1.y > 1.0f ||
                v2.x < -1.0f || v2.x > 1.0f || v2.y < -1.0f || v2.y > 1.0f) continue;

            sf::Vector2f screenPos1 = screenPosition(v1);
            sf::Vector2f screenPos2 = screenPosition(v2);

            sf::Color color = scene.getVerticesEditMode() ? (object->isSelected ? sf::Color::Yellow : sf::Color::White) : sf::Color(255, 255, 255, 64);

            sf::Vertex edgeVertices[] = {
                sf::Vertex(screenPos1, color),
                sf::Vertex(screenPos2, color)
            };

            window.draw(edgeVertices, 2, sf::Lines);
        }

        if (scene.getVerticesEditMode()) {
            int j = 0;
            for (const auto& vertex : projectedVertices) {
                if (vertex.z < NEAR_PLANE || vertex.z > FAR_PLANE) continue;

                sf::Vector2f screenPos = screenPosition(vertex);
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