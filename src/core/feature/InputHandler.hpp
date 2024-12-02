#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "../objects/Object3d.hpp"
#include "../../utils/Matrix4.hpp"

class InputHandler {
public:
    InputHandler(sf::RenderWindow& window, Scene& scene)
        : window(window), scene(scene) {}

    void handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed) {
            handleKeyPressed(event.key.code);
        }

        if (event.type == sf::Event::MouseMoved) {
            handleMouseMoved();
        }
    }

private:
    sf::RenderWindow& window;
    Scene& scene;

    void handleKeyPressed(sf::Keyboard::Key key) {
        Vector3 direction;
        if (scene.getObjects().empty()) {
            return;
        }

        auto& object = scene.getObjects()[0];
        switch (key) {
            case sf::Keyboard::W:
                direction = Vector3(0, 0, 0.1f);
                break;
            case sf::Keyboard::S:
                direction = Vector3(0, 0, -0.1f);
                break;
            case sf::Keyboard::A:
                direction = Vector3(-0.1f, 0, 0);
                break;
            case sf::Keyboard::D:
                direction = Vector3(0.1f, 0, 0);
                break;
            case sf::Keyboard::Q:
                direction = Vector3(0, 0.1f, 0);
                break;
            case sf::Keyboard::E:
                direction = Vector3(0, -0.1f, 0);
                break;
            case sf::Keyboard::I:
                object->rotate(0.05f, 'x');
                break;
            case sf::Keyboard::K:
                object->rotate(-0.05f, 'x');
                break;
            case sf::Keyboard::J:
                object->rotate(-0.05f, 'y');
                break;
            case sf::Keyboard::L:
                object->rotate(0.05f, 'y');
                break;
            case sf::Keyboard::Z:
                object->rotate(0.05f, 'z');
                break;
            case sf::Keyboard::X:
                object->rotate(-0.05f, 'z');
                break;
            case sf::Keyboard::Left:
                object->setTransform(object->transform * Matrix4::translation(-0.05f, 0.0f, 0.0f));
                break;
            case sf::Keyboard::Right:
                object->setTransform(object->transform * Matrix4::translation(0.05f, 0.0f, 0.0f));
                break;
            case sf::Keyboard::Up:
                object->setTransform(object->transform * Matrix4::translation(0.0f, -0.05f, 0.0f));
                break;
            case sf::Keyboard::Down:
                object->setTransform(object->transform * Matrix4::translation(0.0f, 0.05f, 0.0f));
                break;
            default:
                break;
        }

        scene.getCamera().move(direction);
    }

    void handleMouseMoved() {
        static sf::Vector2i lastMousePos = sf::Mouse::getPosition(window);
        sf::Vector2i mouseDelta = sf::Mouse::getPosition(window) - lastMousePos;

        scene.getCamera().rotate(static_cast<float>(mouseDelta.x),
                                 static_cast<float>(mouseDelta.y));

        lastMousePos = sf::Mouse::getPosition(window);
    }
};

#endif
