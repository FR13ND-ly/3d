#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "../../utils/Object3D.hpp"
#include "../../utils/Matrix4.hpp"

class InputHandler {
public:
    InputHandler(sf::RenderWindow& window, Scene& scene)
        : window(window), scene(scene) {}

    void handleEvent(const sf::Event& event) {
        // Handle one-time actions (key presses)
        if (event.type == sf::Event::KeyPressed) {
            if (!scene.getObjects().empty()) {
                auto& object = scene.getObjects()[0];

                switch (event.key.code) {
                    case sf::Keyboard::W:
                        object->rotate(0.05f, 'x');
                    break;
                    case sf::Keyboard::S:
                        object->rotate(-0.05f, 'x');
                    break;
                    case sf::Keyboard::A:
                        object->rotate(-0.05f, 'y');
                    break;
                    case sf::Keyboard::D:
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
            }
        }
    }

private:
    sf::RenderWindow& window;
    Scene& scene;
};

#endif
