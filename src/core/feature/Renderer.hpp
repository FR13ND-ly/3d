#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Camera.hpp"
#include "../objects/Object3D.hpp"
#include <iostream>

class Renderer {
public:
    Renderer(sf::RenderWindow& window) : window(window) {}

    // Render the scene, applying frustum culling to each object
    void render(const Scene& scene) {
        Camera camera = scene.getCamera();          // Get the camera
        Matrix4 viewMatrix = camera.getViewMatrix();  // Get the view matrix from the camera
        Matrix4 projectionMatrix = camera.getProjectionMatrix();  // Get the projection matrix from the camera

        // Iterate over all objects in the scene
        for (const auto& object : scene.getObjects()) {
            Matrix4 modelMatrix = object->transform;
            object->draw(window, modelMatrix, viewMatrix, projectionMatrix);
        }
    }

private:
    sf::RenderWindow& window;  // SFML window used for rendering
};

#endif
