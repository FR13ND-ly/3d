#ifndef CUBE_HPP
#define CUBE_HPP

#include "../../utils/Object3d.hpp"
#include "../../utils/Matrix4.hpp"
#include "../../utils/Vector3.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Cube : public Object3d {
public:
    Cube(float size) : size(size) {
        calculateVertices();
        edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };
    }

    void draw(sf::RenderWindow& window) override {
        Matrix4 projection = Matrix4::perspective(90.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        std::vector<sf::Vector2f> projectedVertices;

        for (const auto& vertex : vertices) {
            Vector3 transformed = this->transform * vertex;
            sf::Vector2f projected(
                transformed.x / (1 - transformed.z / 5),
                transformed.y / (1 - transformed.z / 5)
            );
            projectedVertices.push_back(projected * 200.f + sf::Vector2f(400, 300));
        }

        for (const auto& edge : edges) {
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
        float half = size / 2;
        vertices = {
            { -half, -half, -half }, { half, -half, -half },
            { half,  half, -half }, { -half,  half, -half },
            { -half, -half,  half }, { half, -half,  half },
            { half,  half,  half }, { -half,  half,  half }
        };
    }
};

#endif
