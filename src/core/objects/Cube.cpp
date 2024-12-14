#include "Cube.hpp"
#include <cmath>

Cube::Cube(float sideLength)
    : sideLength(sideLength) {
    calculateVertices();
    calculateEdges();
    calculateFaces();
}

void Cube::calculateVertices() {
    vertices.clear();

    // Calculate half the side length for positioning vertices
    float half = sideLength / 2.0f;

    // Define the 8 vertices of the cube
    // Order follows: bottom face (counter-clockwise), then top face (counter-clockwise)
    vertices = {
        // Bottom face vertices (z = -half)
        Vector3(-half, -half, -half),  // 0
        Vector3(half, -half, -half),   // 1
        Vector3(half, half, -half),    // 2
        Vector3(-half, half, -half),   // 3

        // Top face vertices (z = half)
        Vector3(-half, -half, half),   // 4
        Vector3(half, -half, half),    // 5
        Vector3(half, half, half),     // 6
        Vector3(-half, half, half)     // 7
    };
}

void Cube::calculateFaces() {
    faces.clear();

    // Each face is defined by 4 vertices
    // The last 3 numbers are arbitrary color-like identifiers similar to Sphere implementation
    faces = {
        // Bottom face (z = -half)
        {0, 1, 2, 255, 0, 0},
        {0, 2, 3, 255, 0, 0},

        // Top face (z = half)
        {4, 6, 5, 0, 255, 0},
        {4, 7, 6, 0, 255, 0},

        // Front face (facing positive z, side with x)
        {1, 5, 2, 0, 0, 255},
        {5, 6, 2, 0, 0, 255},

        // Back face (facing negative z, side with x)
        {0, 3, 4, 255, 255, 0},
        {3, 7, 4, 255, 255, 0},

        // Left face (facing negative x side)
        {0, 4, 1, 255, 0, 255},
        {4, 5, 1, 255, 0, 255},

        // Right face (facing positive x side)
        {3, 2, 7, 0, 255, 255},
        {2, 6, 7, 0, 255, 255}
    };
}


void Cube::calculateEdges() {
    edges.clear();

    // Edges connecting bottom face vertices
    edges.push_back({0, 1});
    edges.push_back({1, 2});
    edges.push_back({2, 3});
    edges.push_back({3, 0});

    // Edges connecting top face vertices
    edges.push_back({4, 5});
    edges.push_back({5, 6});
    edges.push_back({6, 7});
    edges.push_back({7, 4});

    // Edges connecting bottom to top
    edges.push_back({0, 4});
    edges.push_back({1, 5});
    edges.push_back({2, 6});
    edges.push_back({3, 7});
}