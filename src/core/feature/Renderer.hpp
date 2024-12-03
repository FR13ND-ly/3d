#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Camera.hpp"
#include "../objects/Object3D.hpp"
#include "../../utils/Matrix4.hpp"
#include "../../utils/Vector3.hpp"
#include "../../utils/Vector4.hpp"
#include <iostream>

class Renderer {
public:
    Renderer(sf::RenderWindow& window) : window(window) {}

    void render(const Scene& scene) {
        Camera camera = scene.getCamera();
        for (const auto& object : scene.getObjects()) {
            renderObject(*object, camera);
        }
    }

private:
    sf::RenderWindow& window;
    void renderObject(Object3d& object, Camera& camera) {
        Matrix4 modelMatrix = object.transform;
        Matrix4 viewMatrix = camera.getViewMatrix();
        Matrix4 projectionMatrix = camera.getProjectionMatrix();

        std::vector<Vector3> transformedVertices;
        for (const auto &vertex : object.getVertices()) {
            Vector4 worldVertex = modelMatrix * Vector4(vertex, 1.0f);
            Vector4 cameraVertex = viewMatrix * worldVertex;
            transformedVertices.push_back(Vector3(cameraVertex.x, cameraVertex.y, cameraVertex.z));
        }

        bool isInsideFrustum = false;
        for (const auto &v : transformedVertices) {
            if (v.z >= 0.1f && v.z < 100.0f) {
                isInsideFrustum = true;
                break;
            }
        }

        if (!isInsideFrustum) {
            return;
        }

        std::vector<Vector3> projectedVertices;
        for (auto &vertex : transformedVertices) {
            Vector4 projectedVertex = projectionMatrix * Vector4(vertex, 1.0f);

            if (projectedVertex.w != 0.0f) {
                projectedVertex.x /= projectedVertex.w;
                projectedVertex.y /= projectedVertex.w;
                projectedVertex.z /= projectedVertex.w;
            }

            projectedVertices.push_back(Vector3(projectedVertex.x, projectedVertex.y, projectedVertex.z));
        }

        for (const auto &edge : object.getEdges()) {
            const auto &p1 = projectedVertices[edge.first];
            const auto &p2 = projectedVertices[edge.second];

            if (p1.z < 0.1f || p2.z < 0.1f) continue;

            sf::Vector2f screenPos1 = {
                (p1.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - p1.y) * 0.5f * window.getSize().y
            };
            sf::Vector2f screenPos2 = {
                (p2.x + 1.0f) * 0.5f * window.getSize().x,
                (1.0f - p2.y) * 0.5f * window.getSize().y
            };

            sf::Vertex line[] = {
                sf::Vertex(screenPos1, sf::Color::White),
                sf::Vertex(screenPos2, sf::Color::White)
            };

            window.draw(line, 2, sf::Lines);
        }
    }
};

#endif
