#ifndef CUBE_HPP
#define CUBE_HPP

#include "Object3D.hpp"
#include "../../utils/Matrix4.hpp"
#include "../../utils/Vector3.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
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

    void draw(sf::RenderWindow& window, const Matrix4& mvp) override {
    std::vector<sf::Vector2f> projectedVertices;
    std::vector<std::pair<int, int>> visibleEdges;


    bool objectVisible = false;

    // std::cout << "==================== \n";
    for (size_t i = 0; i < vertices.size(); ++i) {
        Vector3 transformed = mvp * vertices[i];

        // std::cout << "Transformed Z: " << transformed.z << std::endl;

        // Skip vertices that are behind the camera (z <= 0)
        if (transformed.z <= 0) {
            // std::cout << "Vertex " << i << " is behind the camera, skipping." << std::endl;
            transformed.z = 0.0001f;
        }

        // Set flag if at least one vertex is visible
        objectVisible = true;

        // Perform perspective division (valid only if z > 0)
        transformed.x /= transformed.z;
        transformed.y /= transformed.z;

        // Convert NDC [-1, 1] to screen space
        float width = window.getSize().x;
        float height = window.getSize().y;
        float offsetX = width / 2.0f;
        float offsetY = height / 2.0f;

        projectedVertices.push_back(sf::Vector2f(
            transformed.x * offsetX + offsetX,
            -transformed.y * offsetY + offsetY));
    }
    // std::cout << "==================== \n";

    // If no vertices are visible, don't draw anything
    if (!objectVisible) {
        std::cout << "Object is fully behind the camera, skipping draw." << std::endl;
        return;  // Skip drawing this object
    }

    // Now, loop through the edges and only add visible edges
    for (const auto& edge : edges) {
        int v1 = edge.first;
        int v2 = edge.second;

        // Ensure both vertices are visible (z > 0)
        if (projectedVertices[v1].x != -1 && projectedVertices[v2].x != -1) {
            visibleEdges.push_back(edge);
        }
    }

    // Draw the visible edges (now sorted by z-depth)
    for (const auto& edge : visibleEdges) {
        sf::Vertex line[] = {
            sf::Vertex(projectedVertices[edge.first], sf::Color::White),
            sf::Vertex(projectedVertices[edge.second], sf::Color::White)
        };
        window.draw(line, 2, sf::Lines);
    }
}



private:
    float size;
    std::vector<Vector3> vertices;
    std::vector<std::pair<int, int>> edges;

    void calculateVertices() {
        float half = size / 2.0f;
        vertices = {
            {-half, -half, -half}, {half, -half, -half},
            {half, half, -half},   {-half, half, -half},
            {-half, -half, half},  {half, -half, half},
            {half, half, half},    {-half, half, half}
        };
    }
};

#endif
