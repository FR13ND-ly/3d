#include "Renderer.hpp"
#include <algorithm>
#include "Scene.hpp"
#include "../../utils/WindowManager.hpp"

constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE = 100.0f;

Renderer::Renderer(sf::RenderWindow& window)
    : window(window), frustum() {}

void Renderer::setupFrustum(const Matrix4& projectionMatrix, const Matrix4& viewMatrix) {
    frustum = Frustum(projectionMatrix, viewMatrix);
}

bool Renderer::isFaceCulled(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Matrix4& viewMatrix) const {
    Vector3 edge1 = v2 - v1;
    Vector3 edge2 = v3 - v1;

    Vector3 normal = edge1.cross(edge2);
    if (normal.length() < 1e-6f) {
        return false;
    }
    normal.normalized();

    Vector3 cameraPosition(-viewMatrix(0, 3), -viewMatrix(1, 3), -viewMatrix(2, 3));
    Vector3 viewDirection = (v1 - cameraPosition).normalized();

    float dotProduct = normal.dot(viewDirection);

    return dotProduct > 0.0f;
}


bool Renderer::isObjectInFrustum(const std::vector<Vector3>& vertices) const {
    return std::any_of(vertices.begin(), vertices.end(), [this](const Vector3& v) {
        return frustum.isPointInside(v);
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

    // Improved sorting algorithm considering camera direction
    std::sort(facesToRender.begin(), facesToRender.end(),
        [](const FaceData& a, const FaceData& b) {
            // Calculate centers of faces in view space
            Vector3 aCenterView = (a.v1 + a.v2 + a.v3) * (1.0f / 3.0f);
            Vector3 bCenterView = (b.v1 + b.v2 + b.v3) * (1.0f / 3.0f);

            // Calculate actual distances from camera (squared distance to avoid sqrt)
            float aDistSq = aCenterView.lengthSquared();
            float bDistSq = bDistSq = bCenterView.lengthSquared();

            const float DIST_THRESHOLD_SQ = 0.01f * 0.01f;
            if (std::abs(aDistSq - bDistSq) > DIST_THRESHOLD_SQ) {
                return aDistSq < bDistSq;
            }

            Vector3 aEdge1 = a.v2 - a.v1;
            Vector3 aEdge2 = a.v3 - a.v1;
            Vector3 aNormal = aEdge1.cross(aEdge2).normalized();

            Vector3 bEdge1 = b.v2 - b.v1;
            Vector3 bEdge2 = b.v3 - b.v1;
            Vector3 bNormal = bEdge1.cross(bEdge2).normalized();

            float aDot = aNormal.dot(Vector3(0, 0, 1));
            float bDot = bNormal.dot(Vector3(0, 0, 1));

            if (std::abs(aDot - bDot) > 0.001f) {
                return aDot < bDot;
            }

            return std::less<const Object3d*>()(a.object.get(), b.object.get());
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
    int i = 0;
    for (const auto& triangle : object->getFaces()) {
        float zAvg = (
            cameraSpaceVertices[triangle[0]].z +
            cameraSpaceVertices[triangle[1]].z +
            cameraSpaceVertices[triangle[2]].z
        ) / 3.0f;

        if (isFaceCulled(projectedVertices[triangle[0]],
                         projectedVertices[triangle[1]],
                         projectedVertices[triangle[2]],
                         viewMatrix)) {
            i++;
            continue;
        }

        facesToRender.push_back({
            projectedVertices[triangle[0]],
            projectedVertices[triangle[1]],
            projectedVertices[triangle[2]],
            sf::Color(triangle[3], triangle[4], triangle[5], triangle[6]),
            zAvg,
            object,
            object->isFaceSelected(i),
            object->isFaceHovered(i)
        });
        i++;
    }
}

void Renderer::renderFaces(const std::vector<FaceData>& facesToRender) {
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    int i = 0;
    for (const auto& face : facesToRender) {
        if (face.v1.z < NEAR_PLANE || face.v2.z < NEAR_PLANE || face.v3.z < NEAR_PLANE ||
            face.v1.z > FAR_PLANE || face.v2.z > FAR_PLANE || face.v3.z > FAR_PLANE) {
            continue;
        }

        sf::Vector2f screenPos1 = screenPosition(face.v1);
        sf::Vector2f screenPos2 = screenPosition(face.v2);
        sf::Vector2f screenPos3 = screenPosition(face.v3);

        sf::Color color = scene.getVerticesEditMode() ? sf::Color(255, 255, 255, 32) : face.color;
        if (scene.getFacesEditMode()) {
            if (face.isHovered) {
                color = sf::Color(
                    std::min(face.color.r + 10, 255),
                    std::min(face.color.g + 10, 255),
                    std::max(face.color.b - 5, 0),
                    face.color.a
                );
            }
            if (face.isSelected) {
                color = sf::Color(
                    std::min(face.color.r + 20, 255),
                    std::min(face.color.g + 20, 255),
                    std::max(face.color.b - 10, 0),
                    face.color.a
                );
            }
            if (face.isSelected && face.isHovered) {
                color = sf::Color(
                    std::min(face.color.r + 30, 255),
                    std::min(face.color.g + 30, 255),
                    std::max(face.color.b - 15, 0),
                    face.color.a
                );
            }
        }
        sf::Vertex triangle[] = {
            sf::Vertex(screenPos1, color),
            sf::Vertex(screenPos2, color),
            sf::Vertex(screenPos3, color)
        };

        window.draw(triangle, 3, sf::Triangles);
        i++;
    }
}

void Renderer::renderEdges(const std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera) {
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);

    for (const auto& object : objects) {

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
            sf::Color color = sf::Color(255, 255, 255, 255);
            if (object->isHovered) {
                color = sf::Color(255, 255, 205, 255);
            }
            if (object->isSelected) {
                color = sf::Color(255, 255, 100, 255);
            }
            if (object->isHovered && object->isSelected) {
                color = sf::Color(255, 255, 0, 255);
            }
            if (scene.getVerticesEditMode()) {
                int j = 0;
                for (const auto& vertex : projectedVertices) {
                    if (vertex.z < 0.1f || vertex.z > 100.0f) continue;
                    sf::Vector2f screenPos = {
                        (vertex.x + 1.0f) * 0.5f * window.getSize().x,
                        (1.0f - vertex.y) * 0.5f * window.getSize().y
                    };
                    sf::Color vertexColor = sf::Color(sf::Color(150, 150, 150, 230));
                    if (object->isVertexHovered(j)) {
                        vertexColor = sf::Color(255, 255, 205);
                    }
                    if (object->isVertexSelected(j)) {
                        vertexColor = sf::Color(255, 255, 125);
                    }
                    if (object->isVertexHovered(j) && object->isVertexSelected(j)) {
                        vertexColor = sf::Color(255, 255, 0);
                    }
                    sf::CircleShape vertexCircle(10.0f);
                    vertexCircle.setFillColor(vertexColor);
                    vertexCircle.setPosition(screenPos.x - vertexCircle.getRadius(),
                                             screenPos.y - vertexCircle.getRadius());
                    window.draw(vertexCircle);
                    j++;
                }
            }
            sf::Vertex edgeVertices[] = {
                sf::Vertex(screenPos1, color),
                sf::Vertex(screenPos2, color)
            };

            window.draw(edgeVertices, 2, sf::Lines);
        }
    }
}