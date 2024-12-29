#include "ZBuffer.hpp"
#include "../core/feature/Scene.hpp"

ZBuffer::ZBuffer(sf::RenderWindow& window) : window(window) {}

void ZBuffer::handleClick(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera) {
    if (scene.getVerticesEditMode()) {
        handleVertexSelection(mousePos, scene, camera);
    } else if (scene.getFacesEditMode()) {
        handleFaceSelection(mousePos, scene, camera);
    } else {
        handleObjectSelection(mousePos, scene, camera);
    }
}

void ZBuffer::handleHover(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera) {
    for (auto& object : scene.getObjects()) {
        object->hoveredVertices.clear();
        object->hoveredFaces.clear();
        object->isHovered = false;
    }
    if (scene.getVerticesEditMode()) {
        handleVertexHover(mousePos, scene, camera);
    } else if (scene.getFacesEditMode()) {
        handleFaceHover(mousePos, scene, camera);
    }
    else {
        handleObjectHover(mousePos, scene, camera);
    }
}

void ZBuffer::handleVertexHover(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera) {
    auto& objects = scene.getObjects();
    sf::Vector2f mousePos2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    for (size_t objectIndex = 1; objectIndex < objects.size(); ++objectIndex) {
        auto& object = objects[objectIndex];

        auto modelMatrix = object->getTransformation();
        auto viewMatrix = camera.getViewMatrix();
        auto projectionMatrix = camera.getProjectionMatrix();

        auto vertices = object->getVertices();
        auto projectedVertices = transformAndProjectVertices(vertices, modelMatrix, viewMatrix, projectionMatrix);

        for (size_t i = 0; i < projectedVertices.size(); ++i) {
            sf::Vector2f screenPos = screenPosition(projectedVertices[i]);

            if (getDistance(mousePos2f, screenPos) <= VERTEX_SELECTION_RADIUS) {
                object->hoveredVertices.push_back(i);
                return;
            }
        }
    }
}

void ZBuffer::handleFaceHover(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera) {
    auto& objects = scene.getObjects();
    sf::Vector2f mousePos2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    struct FaceCandidate {
        size_t objectIndex;
        size_t faceIndex;
        float depth;
    };
    std::vector<FaceCandidate> candidates;

    for (size_t objectIndex = 1; objectIndex < objects.size(); ++objectIndex) {
        auto& object = objects[objectIndex];

        auto modelMatrix = object->getTransformation();
        auto viewMatrix = camera.getViewMatrix();
        auto projectionMatrix = camera.getProjectionMatrix();

        auto vertices = object->getVertices();
        auto viewSpaceVertices = transformVertices(vertices, modelMatrix, viewMatrix);
        auto projectedVertices = projectVertices(viewSpaceVertices, projectionMatrix);
        auto faces = object->getFaces();

        for (size_t faceIndex = 0; faceIndex < faces.size(); ++faceIndex) {
            const auto& face = faces[faceIndex];
            sf::Vector2f v1Screen = screenPosition(projectedVertices[face[0]]);
            sf::Vector2f v2Screen = screenPosition(projectedVertices[face[1]]);
            sf::Vector2f v3Screen = screenPosition(projectedVertices[face[2]]);

            if (isPointInTriangle(mousePos2f, v1Screen, v2Screen, v3Screen)) {
                Vector3 v1View = viewSpaceVertices[face[0]];
                Vector3 v2View = viewSpaceVertices[face[1]];
                Vector3 v3View = viewSpaceVertices[face[2]];

                // Calculate face normal in view space
                Vector3 edge1 = v2View - v1View;
                Vector3 edge2 = v3View - v1View;
                Vector3 normal = edge1.cross(edge2).normalized();

                if (normal.z > 0) continue;

                float alpha = calculateBarycentric(mousePos2f, v1Screen, v2Screen, v3Screen);
                float beta = calculateBarycentric(mousePos2f, v2Screen, v3Screen, v1Screen);
                float gamma = 1.0f - alpha - beta;

                float depth = alpha * v1View.z + beta * v2View.z + gamma * v3View.z;
                candidates.push_back({objectIndex, faceIndex, depth});
            }
        }
    }

    std::sort(candidates.begin(), candidates.end(),
        [](const FaceCandidate& a, const FaceCandidate& b) {
            return a.depth < b.depth;
        });

    if (!candidates.empty()) {
        auto& object = objects[candidates[0].objectIndex];
        object->hoveredFaces.push_back(candidates[0].faceIndex);
    }
}

void ZBuffer::handleObjectHover(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera) {
    auto& objects = scene.getObjects();
    sf::Vector2f mousePos2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    struct FaceCandidate {
        size_t objectIndex;
        float depth;
    };
    std::vector<FaceCandidate> candidates;

    for (size_t i = 1; i < objects.size(); ++i) {
        auto& object = objects[i];
        auto modelMatrix = object->getTransformation();
        auto viewMatrix = camera.getViewMatrix();
        auto projectionMatrix = camera.getProjectionMatrix();

        auto vertices = object->getVertices();
        auto viewSpaceVertices = transformVertices(vertices, modelMatrix, viewMatrix);
        auto projectedVertices = projectVertices(viewSpaceVertices, projectionMatrix);
        auto faces = object->getFaces();

        for (const auto& face : faces) {
            sf::Vector2f v1Screen = screenPosition(projectedVertices[face[0]]);
            sf::Vector2f v2Screen = screenPosition(projectedVertices[face[1]]);
            sf::Vector2f v3Screen = screenPosition(projectedVertices[face[2]]);

            if (isPointInTriangle(mousePos2f, v1Screen, v2Screen, v3Screen)) {
                Vector3 v1View = viewSpaceVertices[face[0]];
                Vector3 v2View = viewSpaceVertices[face[1]];
                Vector3 v3View = viewSpaceVertices[face[2]];

                // Calculate face normal in view space
                Vector3 edge1 = v2View - v1View;
                Vector3 edge2 = v3View - v1View;
                Vector3 normal = edge1.cross(edge2).normalized();

                // Skip back-facing triangles
                if (normal.z > 0) continue;

                float alpha = calculateBarycentric(mousePos2f, v1Screen, v2Screen, v3Screen);
                float beta = calculateBarycentric(mousePos2f, v2Screen, v3Screen, v1Screen);
                float gamma = 1.0f - alpha - beta;

                float depth = alpha * v1View.z + beta * v2View.z + gamma * v3View.z;
                candidates.push_back({i, depth});
            }
        }
    }

    std::sort(candidates.begin(), candidates.end(),
        [](const FaceCandidate& a, const FaceCandidate& b) {
            return a.depth < b.depth;
        });

    if (!candidates.empty()) {
        objects[candidates[0].objectIndex]->isHovered = true;
    }
}

void ZBuffer::handleVertexSelection(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera) {
    auto& objects = scene.getObjects();
    sf::Vector2f mousePos2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    for (size_t objectIndex = 1; objectIndex < objects.size(); ++objectIndex) {
        auto& object = objects[objectIndex];

        auto modelMatrix = object->getTransformation();
        auto viewMatrix = camera.getViewMatrix();
        auto projectionMatrix = camera.getProjectionMatrix();

        auto vertices = object->getVertices();
        auto projectedVertices = transformAndProjectVertices(vertices, modelMatrix, viewMatrix, projectionMatrix);

        for (size_t i = 0; i < projectedVertices.size(); ++i) {
            sf::Vector2f screenPos = screenPosition(projectedVertices[i]);

            if (getDistance(mousePos2f, screenPos) <= VERTEX_SELECTION_RADIUS) {
                // Update the selected object index
                scene.onChangeSelectedObjectIndex(objectIndex);

                // Toggle vertex selection
                auto& selectedVertices = object->selectedVertices;
                auto it = std::find(selectedVertices.begin(), selectedVertices.end(), i);

                if (it != selectedVertices.end()) {
                    selectedVertices.erase(it);
                } else {
                    selectedVertices.push_back(i);
                }
                return;
            }
        }
    }
}

void ZBuffer::handleFaceSelection(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera) {
    auto& objects = scene.getObjects();
    sf::Vector2f mousePos2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    // Structure to store face candidates that contain the clicked point
    struct FaceCandidate {
        size_t objectIndex;
        size_t faceIndex;
        float depth;
    };
    std::vector<FaceCandidate> candidates;

    for (size_t objectIndex = 1; objectIndex < objects.size(); ++objectIndex) {
        auto& object = objects[objectIndex];

        auto modelMatrix = object->getTransformation();
        auto viewMatrix = camera.getViewMatrix();
        auto projectionMatrix = camera.getProjectionMatrix();

        // Transform vertices to view space for depth checking
        auto vertices = object->getVertices();
        auto viewSpaceVertices = transformVertices(vertices, modelMatrix, viewMatrix);
        auto projectedVertices = projectVertices(viewSpaceVertices, projectionMatrix);
        auto faces = object->getFaces();

        for (size_t faceIndex = 0; faceIndex < faces.size(); ++faceIndex) {
            const auto& face = faces[faceIndex];
            sf::Vector2f v1Screen = screenPosition(projectedVertices[face[0]]);
            sf::Vector2f v2Screen = screenPosition(projectedVertices[face[1]]);
            sf::Vector2f v3Screen = screenPosition(projectedVertices[face[2]]);

            if (isPointInTriangle(mousePos2f, v1Screen, v2Screen, v3Screen)) {
                // Get the actual view space positions for precise depth calculation
                Vector3 v1View = viewSpaceVertices[face[0]];
                Vector3 v2View = viewSpaceVertices[face[1]];
                Vector3 v3View = viewSpaceVertices[face[2]];

                // Calculate the face normal in view space
                Vector3 edge1 = v2View - v1View;
                Vector3 edge2 = v3View - v1View;
                Vector3 normal = edge1.cross(edge2).normalized();

                // Skip back-facing triangles
                if (normal.z > 0) continue;

                // Calculate barycentric coordinates for interpolation
                float alpha = calculateBarycentric(mousePos2f, v1Screen, v2Screen, v3Screen);
                float beta = calculateBarycentric(mousePos2f, v2Screen, v3Screen, v1Screen);
                float gamma = 1.0f - alpha - beta;

                // Interpolate Z value at click point
                float depth = alpha * v1View.z + beta * v2View.z + gamma * v3View.z;

                candidates.push_back({objectIndex, faceIndex, depth});
            }
        }
    }

    // Sort candidates by depth (closest to camera first)
    std::sort(candidates.begin(), candidates.end(),
        [](const FaceCandidate& a, const FaceCandidate& b) {
            return a.depth < b.depth;
        });

    // Select the front-most face
    if (!candidates.empty()) {
        auto& object = objects[candidates[0].objectIndex];
        scene.onChangeSelectedObjectIndex(candidates[0].objectIndex);

        auto& selectedFaces = object->selectedFaces;
        auto it = std::find(selectedFaces.begin(), selectedFaces.end(), candidates[0].faceIndex);

        if (it != selectedFaces.end()) {
            selectedFaces.erase(it);
        } else {
            selectedFaces.push_back(candidates[0].faceIndex);
        }
    }
}


void ZBuffer::handleObjectSelection(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera) {
    auto& objects = scene.getObjects();
    sf::Vector2f mousePos2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

    struct FaceCandidate {
        size_t objectIndex;
        float depth;
    };
    std::vector<FaceCandidate> candidates;

    for (size_t i = 1; i < objects.size(); ++i) {
        auto& object = objects[i];
        auto modelMatrix = object->getTransformation();
        auto viewMatrix = camera.getViewMatrix();
        auto projectionMatrix = camera.getProjectionMatrix();

        auto vertices = object->getVertices();
        auto viewSpaceVertices = transformVertices(vertices, modelMatrix, viewMatrix);
        auto projectedVertices = projectVertices(viewSpaceVertices, projectionMatrix);
        auto faces = object->getFaces();

        for (const auto& face : faces) {
            sf::Vector2f v1Screen = screenPosition(projectedVertices[face[0]]);
            sf::Vector2f v2Screen = screenPosition(projectedVertices[face[1]]);
            sf::Vector2f v3Screen = screenPosition(projectedVertices[face[2]]);

            if (isPointInTriangle(mousePos2f, v1Screen, v2Screen, v3Screen)) {
                Vector3 v1View = viewSpaceVertices[face[0]];
                Vector3 v2View = viewSpaceVertices[face[1]];
                Vector3 v3View = viewSpaceVertices[face[2]];

                Vector3 edge1 = v2View - v1View;
                Vector3 edge2 = v3View - v1View;
                Vector3 normal = edge1.cross(edge2).normalized();

                if (normal.z > 0) continue;

                float alpha = calculateBarycentric(mousePos2f, v1Screen, v2Screen, v3Screen);
                float beta = calculateBarycentric(mousePos2f, v2Screen, v3Screen, v1Screen);
                float gamma = 1.0f - alpha - beta;

                float depth = alpha * v1View.z + beta * v2View.z + gamma * v3View.z;
                candidates.push_back({i, depth});
                break;
            }
        }
    }

    std::sort(candidates.begin(), candidates.end(),
        [](const FaceCandidate& a, const FaceCandidate& b) {
            return a.depth < b.depth;
        });

    if (!candidates.empty()) {
        if (scene.getObjectsEditMode()) {
            scene.toggleObjectSelected(candidates[0].objectIndex);
        } else {
            for (int i = 0; i < objects.size(); i++) {
                objects[i]->isSelected = false;
            }
            scene.selectedObjects.clear();
            scene.selectedObjects.push_back(candidates[0].objectIndex);
            objects[candidates[0].objectIndex]->isSelected = true;
            scene.onChangeSelectedObjectIndex(candidates[0].objectIndex);
        }
    }
}

std::vector<Vector3> ZBuffer::transformAndProjectVertices(
    const std::vector<Vector3>& vertices,
    const Matrix4& modelMatrix,
    const Matrix4& viewMatrix,
    const Matrix4& projectionMatrix) const {

    std::vector<Vector3> result;
    result.reserve(vertices.size());

    for (const auto& vertex : vertices) {
        Vector4 worldSpace = modelMatrix * Vector4(vertex, 1.0f);
        Vector4 viewSpace = viewMatrix * worldSpace;
        Vector4 clipSpace = projectionMatrix * viewSpace;

        if (clipSpace.w != 0.0f) {
            clipSpace.x /= clipSpace.w;
            clipSpace.y /= clipSpace.w;
            clipSpace.z /= clipSpace.w;
        }

        result.emplace_back(clipSpace.x, clipSpace.y, clipSpace.z);
    }

    return result;
}

sf::Vector2f ZBuffer::screenPosition(const Vector3& vertex) const {
    return {
        (vertex.x + 1.0f) * 0.5f * window.getSize().x,
        (1.0f - vertex.y) * 0.5f * window.getSize().y
    };
}

float ZBuffer::getDistance(const sf::Vector2f& p1, const sf::Vector2f& p2) const {
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    return std::sqrt(dx * dx + dy * dy);
}

bool ZBuffer::isPointInTriangle(const sf::Vector2f& p, const sf::Vector2f& a,
                              const sf::Vector2f& b, const sf::Vector2f& c) const {
    float alpha = calculateBarycentric(p, a, b, c);
    float beta = calculateBarycentric(p, b, c, a);
    float gamma = calculateBarycentric(p, c, a, b);

    // Point is inside if all barycentric coordinates are positive
    return alpha >= 0.0f && beta >= 0.0f && gamma >= 0.0f;
}

float ZBuffer::calculateBarycentric(const sf::Vector2f& p, const sf::Vector2f& a,
                                  const sf::Vector2f& b, const sf::Vector2f& c) const {
    return ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) /
           ((b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y));
}

std::vector<Vector3> ZBuffer::transformVertices(const std::vector<Vector3>& vertices,
                                              const Matrix4& modelMatrix,
                                              const Matrix4& viewMatrix) const {
    std::vector<Vector3> result;
    result.reserve(vertices.size());

    for (const auto& vertex : vertices) {
        Vector4 worldSpace = modelMatrix * Vector4(vertex, 1.0f);
        Vector4 viewSpace = viewMatrix * worldSpace;
        result.emplace_back(viewSpace.x, viewSpace.y, viewSpace.z);
    }

    return result;
}

std::vector<Vector3> ZBuffer::projectVertices(const std::vector<Vector3>& viewSpaceVertices,
                                            const Matrix4& projectionMatrix) const {
    std::vector<Vector3> result;
    result.reserve(viewSpaceVertices.size());

    for (const auto& vertex : viewSpaceVertices) {
        Vector4 clipSpace = projectionMatrix * Vector4(vertex, 1.0f);

        if (clipSpace.w != 0.0f) {
            clipSpace.x /= clipSpace.w;
            clipSpace.y /= clipSpace.w;
            clipSpace.z /= clipSpace.w;
        }

        result.emplace_back(clipSpace.x, clipSpace.y, clipSpace.z);
    }

    return result;
}