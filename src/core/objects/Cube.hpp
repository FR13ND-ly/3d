#ifndef CUBE_HPP
#define CUBE_HPP

#include "Object3D.hpp"
#include "../../utils/Matrix4.hpp"
#include "../../utils/Vector3.hpp"
#include "../../utils/Vector4.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

class Cube : public Object3d {
public:
    Cube(float size) : size(size) {
        calculateVertices();
        edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Front face
            {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Back face
            {0, 4}, {1, 5}, {2, 6}, {3, 7}, // Connecting edges
        };
    }

    void draw(sf::RenderWindow &window, const Matrix4 &modelMatrix, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix) override {
    std::vector<Vector3> transformedVertices;

    // Step 1: Transform vertices from object space to world space using the model matrix
    for (const auto &vertex : vertices) {
        Vector4 worldVertex = modelMatrix * Vector4(vertex, 1.0f);

        // Step 2: Transform world space vertex to camera space using the view matrix
        Vector4 cameraVertex = viewMatrix * worldVertex;

        // Store in transformed vertices list for frustum check
        transformedVertices.push_back(Vector3(cameraVertex.x, cameraVertex.y, cameraVertex.z));
    }

    // Step 3: Check if any vertex is inside the view frustum (near and far plane)
    bool isInsideFrustum = false;
    for (const auto &v : transformedVertices) {
        if (v.z >= 0.1f && v.z < 100.0f) {  // Near and far clipping planes
            isInsideFrustum = true;
            break;
        }
    }

    if (!isInsideFrustum) {
        return;  // Skip rendering if the object is completely outside the view frustum
    }

    // Step 4: Project the vertices from camera space to screen space
    std::vector<Vector3> projectedVertices;
    for (auto &vertex : transformedVertices) {
        Vector4 projectedVertex = projectionMatrix * Vector4(vertex, 1.0f);

        if (projectedVertex.w != 0.0f) {
            projectedVertex.x /= projectedVertex.w;
            projectedVertex.y /= projectedVertex.w;
            projectedVertex.z /= projectedVertex.w;
        }

        // Store the projected vertex
        projectedVertices.push_back(Vector3(projectedVertex.x, projectedVertex.y, projectedVertex.z));
    }

    // Step 5: Render edges if they are within the frustum and visible
    for (const auto &edge : edges) {
        const auto &p1 = projectedVertices[edge.first];
        const auto &p2 = projectedVertices[edge.second];

        if (p1.z < 0.1f || p2.z < 0.1f) continue;  // Skip edges behind the near plane

        sf::Vector2f screenPos1 = {
            (p1.x + 1.0f) * 0.5f * window.getSize().x,
            (1.0f - p1.y) * 0.5f * window.getSize().y
        };
        sf::Vector2f screenPos2 = {
            (p2.x + 1.0f) * 0.5f * window.getSize().x,
            (1.0f - p2.y) * 0.5f * window.getSize().y
        };

        // Draw the edge
        sf::Vertex line[] = {
            sf::Vertex(screenPos1, sf::Color::White),
            sf::Vertex(screenPos2, sf::Color::White)
        };
        window.draw(line, 2, sf::Lines);
    }
}



private:
    float size;
    std::vector<Vector3> vertices;
    std::vector<std::pair<int, int> > edges;

    void calculateVertices() {
        float half = size / 2.0f;
        vertices = {
            {-half, -half, -half}, {half, -half, -half},
            {half, half, -half}, {-half, half, -half},
            {-half, -half, half}, {half, -half, half},
            {half, half, half}, {-half, half, half}
        };
    }
};

#endif
