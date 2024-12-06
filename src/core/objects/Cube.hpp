#ifndef CUBE_HPP
#define CUBE_HPP

#include "Object3D.hpp"
#include "../../utils/Vector3.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Cube : public Object3d {
public:
    Cube(float size) : size(size) {
        calculateVertices();
        calculateFaces();
        calculateEdges();
    }

    std::vector<std::array<int, 6>> getSortedFaces(const std::vector<Vector3>& transformedVertices) const override {
        std::vector<std::tuple<float, std::array<int, 6>, std::array<int, 6>>> faceDepths;

        for (size_t i = 0; i < faces.size(); i += 2) {
            const auto& triangle1 = faces[i];
            const auto& triangle2 = faces[i + 1];

            float zAvg = (
                transformedVertices[triangle1[0]].z +
                transformedVertices[triangle1[1]].z +
                transformedVertices[triangle1[2]].z +
                transformedVertices[triangle2[0]].z +
                transformedVertices[triangle2[1]].z +
                transformedVertices[triangle2[2]].z
            ) / 6.0f;

            faceDepths.emplace_back(zAvg, triangle1, triangle2);
        }

        std::sort(faceDepths.begin(), faceDepths.end(), [](const auto& a, const auto& b) {
            return std::get<0>(a) > std::get<0>(b);
        });

        std::vector<std::array<int, 6>> sortedFaces;
        for (const auto& [_, triangle1, triangle2] : faceDepths) {
            sortedFaces.push_back(triangle1);
            sortedFaces.push_back(triangle2);
        }

        return sortedFaces;
    }

    float size;

private:
    void calculateVertices() {
        float half = size / 2.0f;
        vertices = {
            {-half, -half, -half}, {half, -half, -half},
            {half, half, -half}, {-half, half, -half},
            {-half, -half, half}, {half, -half, half},
            {half, half, half}, {-half, half, half},
            // Additional vertices for more detail if needed
        };
    }

    void calculateFaces() {
        faces.clear();

        // Front face (z = -1)
        faces.push_back({0, 1, 2, 255, 0, 0});
        faces.push_back({0, 2, 3, 255, 0, 0});

        // Back face (z = 1)
        faces.push_back({4, 5, 6, 0, 255, 0});
        faces.push_back({4, 6, 7, 0, 255, 0});

        // Left face (x = -1)
        faces.push_back({0, 4, 7, 0, 0, 255});
        faces.push_back({0, 7, 3, 0, 0, 255});

        // Right face (x = 1)
        faces.push_back({1, 5, 6, 255, 255, 0});
        faces.push_back({1, 6, 2, 255, 255, 0});

        // Top face (y = 1)
        faces.push_back({3, 2, 6, 255, 0, 255});
        faces.push_back({3, 6, 7, 255, 0, 255});

        // Bottom face (y = -1)
        faces.push_back({0, 1, 5, 0, 255, 255});
        faces.push_back({0, 5, 4, 0, 255, 255});

        // Additional faces can be added here for detail if needed
    }

    void calculateEdges() {
        edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7},
        };
    }
};

#endif
