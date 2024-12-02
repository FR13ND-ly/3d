#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "../objects/Ground.hpp"
#include <iostream>

class Renderer {
public:
    Renderer(sf::RenderWindow& window) : window(window) {}

    void render(const Scene& scene) {
        Matrix4 viewMatrix = scene.getCamera().getViewMatrix();
        Matrix4 projectionMatrix = scene.getCamera().getProjectionMatrix();

        for (const auto& object : scene.getObjects()) {
            Matrix4 modelMatrix = object->transform;
            Matrix4 mvp = projectionMatrix * viewMatrix * modelMatrix;
            object->draw(window, mvp);
        }
    }


private:
    sf::RenderWindow& window;
};

#endif