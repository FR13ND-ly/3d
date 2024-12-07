#ifndef SCENE_HPP
#define SCENE_HPP

#include "../objects/Object3d.hpp"
#include "../ui/Component.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include <vector>
#include <memory>

class Scene: public Component {
public:
    Scene(sf::RenderWindow& window)
        : camera(window.getSize().x, window.getSize().y, 90.0f, 0.1f, 100.0f), renderer(window), window(window)  {
    }

    void addObject(std::shared_ptr<Object3d> object) {
        objects.push_back(object);
        onChangeSelectedObjectIndex(objects.size() - 1);
    }

    const std::vector<std::shared_ptr<Object3d>>& getObjects() const {
        return objects;
    }

    void setCamera(const Camera& camera) {
        this->camera = camera;
    }

    const Camera& getCamera() const {
        return camera;
    }

    Camera& getCamera() {
        return camera;
    }

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override {
        if (event.type == sf::Event::KeyPressed) {
            handleKeyPressed(event.key.code);
        }

        if (event.type == sf::Event::MouseMoved) {
            handleMouseMoved();
        }

        if (event.type == sf::Event::MouseWheelScrolled) {
            handleScroll(event.mouseWheelScroll);
        }
    }

    void draw(sf::RenderWindow& window) override {
        renderer.render(objects, camera);
    }



    void onChangeSelectedObjectIndex(int selectedObjectIndex) {
        for (int i = 0; i < objects.size(); i++) {
            objects[i]->isSelected = false;
        }
        this->selectedObjectIndex = selectedObjectIndex;
        objects[selectedObjectIndex]->isSelected = true;
    }

private:
    std::vector<std::shared_ptr<Object3d>> objects;
    Camera camera;
    Renderer renderer;
    sf::RenderWindow& window;
    int selectedObjectIndex = 0;

    void handleScroll(sf::Event::MouseWheelScrollEvent event) {
        Vector3 direction;
        direction = Vector3(0, 0, 0.3f * event.delta);
        getCamera().move(direction);
    }

    void handleKeyPressed(sf::Keyboard::Key key) {
        Vector3 direction;
        if (getObjects().empty()) {
            return;
        }

        auto& object = getObjects()[selectedObjectIndex];
        switch (key) {
            case sf::Keyboard::Num1:
                onChangeSelectedObjectIndex(0);
            break;
            case sf::Keyboard::Num2:
                onChangeSelectedObjectIndex(1);
            break;
            case sf::Keyboard::W:;
                camera.orbitPitch(-10.0f);
                break;
            case sf::Keyboard::S:
                camera.orbitPitch(10.0f);
                break;
            case sf::Keyboard::A:
                camera.orbitYaw(10.0f);
                // direction = Vector3(-0.1f, 0, 0);
                break;
            case sf::Keyboard::D:
                camera.orbitYaw(-10.0f);
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
            case sf::Keyboard::R:
                // camera.lookAtCenter();
            break;
            default:
                break;
        }

        getCamera().move(direction);
    }

    void handleMouseMoved() {
        static bool isMiddleMouseHeld = false;
        static sf::Vector2i lastMousePos; // To store the last mouse position

        // Check if the middle mouse button is held
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
            if (!isMiddleMouseHeld) {
                isMiddleMouseHeld = true;
                window.setMouseCursorVisible(false);
                lastMousePos = sf::Mouse::getPosition(window); // Initialize lastMousePos
            }

            sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
            sf::Vector2i mouseDelta = currentMousePos - lastMousePos;

            if (mouseDelta != sf::Vector2i(0, 0)) {
                float sensitivity = 0.1f; // Adjust sensitivity as needed

                getCamera().rotate(
                    sensitivity * static_cast<float>(mouseDelta.x),
                    sensitivity * static_cast<float>(mouseDelta.y));

                lastMousePos = currentMousePos;
            }
        } else {
            // Reset when the middle mouse button is released
            if (isMiddleMouseHeld) {
                isMiddleMouseHeld = false;
                window.setMouseCursorVisible(true);
            }
        }
    }


};

#endif
