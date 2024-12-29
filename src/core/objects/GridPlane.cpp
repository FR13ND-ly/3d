#include "GridPlane.hpp"

GridPlane::GridPlane() {
    calculateVertices();
    calculateEdges();
    calculateFaces();
}

void GridPlane::calculateVertices() {
    vertices.clear();
    float halfGrid = gridSize / 2.0f;

    for (float z = -halfGrid; z <= halfGrid; z += cellSize) {
        for (float x = -halfGrid; x <= halfGrid; x += cellSize) {
            vertices.emplace_back(x, 0.0f, z);
        }
    }
}

void GridPlane::calculateEdges() {
    edges.clear();
    // int rows = static_cast<int>(gridSize / cellSize) + 1;
    //
    // for (int z = 0; z < rows; ++z) {
    //     for (int x = 0; x < rows; ++x) {
    //         int currentIndex = z * rows + x;
    //
    //         // Add horizontal edge
    //         if (x < rows - 1) {
    //             edges.emplace_back(currentIndex, currentIndex + 1);
    //         }
    //
    //         // Add vertical edge
    //         if (z < rows - 1) {
    //             edges.emplace_back(currentIndex, currentIndex + rows);
    //         }
    //     }
    // }
}

void GridPlane::calculateFaces() {
    faces.clear();
    int rows = static_cast<int>(gridSize / cellSize) + 1;

    for (int z = 0; z < rows; ++z) {
        for (int x = 0; x < rows; ++x) {
            int currentIndex = z * rows + x;

            if (z < rows - 1) {
                int nextIndexZ = currentIndex + rows;
                addLineFace(currentIndex, nextIndexZ, 'z');
            }

            if (x < rows - 1) {
                int nextIndexX = currentIndex + 1;
                addLineFace(currentIndex, nextIndexX, 'x');
            }
        }
    }
}

void GridPlane::addLineFace(int startIndex, int endIndex, char axis) {
    Vector3 start = vertices[startIndex];
    Vector3 end = vertices[endIndex];
    Vector3 offset;

    if (axis == 'x') {
        offset = Vector3(0.0f, lineThickness / 2.0f, 0.0f); // Y-axis offset for vertical lines
    } else if (axis == 'z') {
        offset = Vector3(lineThickness / 2.0f, 0.0f, 0.0f); // X-axis offset for horizontal lines
    }

    int v0 = vertices.size();
    vertices.push_back(start - offset);
    vertices.push_back(start + offset);
    vertices.push_back(end - offset);
    vertices.push_back(end + offset);

    // Check if the line is on the x-axis or z-axis
    bool isXAxisLine = (start.z == 0.0f && end.z == 0.0f);
    bool isZAxisLine = (start.x == 0.0f && end.x == 0.0f);

    // Set the color based on the axis
    int r = 128, g = 128, b = 128;
    if (isXAxisLine) {
        r = 0; g = 255; b = 0; // Green for x-axis
    } else if (isZAxisLine) {
        r = 255; g = 0; b = 0; // Red for z-axis
    }

    faces.push_back({v0, v0 + 2, v0 + 1, r, g, b, 155}); // Front face
    faces.push_back({v0 + 1, v0 + 2, v0 + 3, r, g, b, 155}); // Front face

    faces.push_back({v0 + 1, v0 + 2, v0, r, g, b, 155}); // Back face
    faces.push_back({v0 + 3, v0 + 2, v0 + 1, r, g, b, 155}); // Back face
}

