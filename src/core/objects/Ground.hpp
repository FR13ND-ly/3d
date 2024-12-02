#ifndef GROUND_HPP
#define GROUND_HPP

#include "Object3D.hpp"

class Ground : public Object3d {
public:
    Ground(float size, int gridSpacing = 10)
        : size(size), gridSpacing(gridSpacing) {
        calculateVertices();
        // Set the ground to have no transformation so it stays stationary
        transform = Matrix4::identity(); // Identity matrix means no transformation
    }

    void draw(sf::RenderWindow& window, const Matrix4& mvp) override {
        std::vector<sf::Vector2f> projectedVertices;

        // Project all vertices
        for (const auto& vertex : vertices) {
            Vector3 transformed = mvp * vertex;

            if (transformed.z > 0) { // Avoid division by zero and inverted depth
                transformed.x /= transformed.z;
                transformed.y /= transformed.z;
            }

            // Convert NDC [-1, 1] to screen space
            float width = window.getSize().x;
            float height = window.getSize().y;
            float offsetX = width / 2.0f;
            float offsetY = height / 2.0f;

            projectedVertices.push_back(sf::Vector2f(
                transformed.x * offsetX + offsetX,
                -transformed.y * offsetY + offsetY));
        }

        // Draw the grid for the ground
        for (const auto& line : gridLines) {
            sf::VertexArray lineArray(sf::Lines, 2);
            lineArray[0].position = projectedVertices[line.first];
            lineArray[1].position = projectedVertices[line.second];

            // Darker grey color for grid lines
            lineArray[0].color = sf::Color(100, 100, 100); // Dark grey
            lineArray[1].color = sf::Color(100, 100, 100);

            window.draw(lineArray);
        }
    }

private:
    float size;
    int gridSpacing;  // Space between grid lines
    std::vector<Vector3> vertices;
    std::vector<std::pair<int, int>> gridLines;

    void calculateVertices() {
        // The vertices will form a square plane in the XY plane
        vertices = {
            {-size, 0, -size}, {size, 0, -size},   // Bottom left, bottom right
            {size, 0, size},  {-size, 0, size}      // Top right, top left
        };

        // Add grid lines in the X and Z directions to form a large grid
        for (float x = -size; x <= size; x += gridSpacing) {
            // Vertical lines (X direction)
            gridLines.push_back({getVertexIndex(x, -size), getVertexIndex(x, size)});
        }

        for (float z = -size; z <= size; z += gridSpacing) {
            // Horizontal lines (Z direction)
            gridLines.push_back({getVertexIndex(-size, z), getVertexIndex(size, z)});
        }
    }

    // Helper function to find the index of a vertex (either X or Z)
    int getVertexIndex(float x, float z) {
        for (int i = 0; i < 4; ++i) {
            if (vertices[i].x == x && vertices[i].z == z) {
                return i;
            }
        }
        return -1;  // Should not happen if the vertices are set correctly
    }
};

#endif
