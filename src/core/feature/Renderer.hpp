#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "../../utils/Object3d.hpp"
#include "Scene.hpp"

class Renderer {
public:
    Renderer(sf::RenderWindow& window) : window(window) {}

    void render(const Scene& scene) {
        for (const auto& object : scene.getObjects()) {
            object->draw(window);
        }
    }

private:
    sf::RenderWindow& window;
};

#endif
