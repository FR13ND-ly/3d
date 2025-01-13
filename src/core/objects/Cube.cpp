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

    float half = sideLength / 2.0f;

    vertices = {
        Vector3(-half, -half, -half),
        Vector3(half, -half, -half),
        Vector3(half, half, -half),
        Vector3(-half, half, -half),

        Vector3(-half, -half, half),
        Vector3(half, -half, half),
        Vector3(half, half, half),
        Vector3(-half, half, half)
    };
}

void Cube::calculateFaces() {
    faces.clear();

    faces = {
        {0, 1, 2, 255, 0, 0},
        {0, 2, 3, 255, 0, 0},

        {4, 6, 5, 0, 255, 0},
        {4, 7, 6, 0, 255, 0},

        {1, 5, 2, 0, 0, 255},
        {5, 6, 2, 0, 0, 255},

        {0, 3, 4, 255, 255, 0},
        {3, 7, 4, 255, 255, 0},

        {0, 4, 1, 255, 0, 255},
        {4, 5, 1, 255, 0, 255},

        {3, 2, 7, 0, 255, 255},
        {2, 6, 7, 0, 255, 255}
    };
}


void Cube::calculateEdges() {
    edges.clear();

    edges.push_back({0, 1});
    edges.push_back({1, 2});
    edges.push_back({2, 3});
    edges.push_back({3, 0});

    edges.push_back({4, 5});
    edges.push_back({5, 6});
    edges.push_back({6, 7});
    edges.push_back({7, 4});

    edges.push_back({0, 4});
    edges.push_back({1, 5});
    edges.push_back({2, 6});
    edges.push_back({3, 7});
}