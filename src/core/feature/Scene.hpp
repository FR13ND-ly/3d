#ifndef SCENE_HPP
#define SCENE_HPP

#include "../objects/Object3d.hpp"
#include "../objects/GridPlane.hpp"
#include "../ui/Component.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include <vector>
#include <memory>

class Scene : public Component {
public:
    Scene(sf::RenderWindow &window)
        : camera(window.getSize().x, window.getSize().y, 90.0f, 0.1f, 100.0f), renderer(window), window(window),
          yawVelocity(0.0f), pitchVelocity(0.0f), dampingFactor(0.95f) {
        addObject(std::shared_ptr<Object3d>(new GridPlane()));
    }

    void addObject(std::shared_ptr<Object3d> object) {
        objects.push_back(object);
        onChangeSelectedObjectIndex(objects.size() - 1);
    }

    const std::vector<std::shared_ptr<Object3d> > &getObjects() const {
        return objects;
    }

    void setCamera(const Camera &camera) {
        // this->camera = camera;
    }

    const Camera &getCamera() const {
        return camera;
    }

    Camera &getCamera() {
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

    void draw(sf::RenderWindow &window) override {
        if (inertiaEnabled) {
            sf::Time dt = clock.restart();
            float deltaTime = dt.asSeconds();

            update(deltaTime);
        }
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
    std::vector<std::shared_ptr<Object3d> > objects;
    Camera camera;
    Renderer renderer;
    sf::RenderWindow &window;
    int selectedObjectIndex = 0;

    float yawVelocity;
    float pitchVelocity;
    float dampingFactor;
    sf::Clock clock;
    bool inertiaEnabled = true;

    void setInertiaEnabled(bool enabled) {
        inertiaEnabled = enabled;
    }

    void handleScroll(sf::Event::MouseWheelScrollEvent event) {
        camera.zoom(0.5f * event.delta);
    }

    void handleKeyPressed(sf::Keyboard::Key key) {
        Vector3 direction;
        if (getObjects().empty()) {
            return;
        }

        auto &object = getObjects()[selectedObjectIndex];
        switch (key) {
            case sf::Keyboard::Num1:
                onChangeSelectedObjectIndex(0);
                break;
            case sf::Keyboard::Num2:
                onChangeSelectedObjectIndex(1);
                break;
            case sf::Keyboard::W:
                camera.rotatePitch(1.5f);
                break;
            case sf::Keyboard::S:
                camera.rotatePitch(-1.5f);
                break;
            case sf::Keyboard::A:
                camera.rotateYaw(-1.5f);
                break;
            case sf::Keyboard::D:
                camera.rotateYaw(1.5f);
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

        getCamera().move(direction);
    }

    void handleMouseMoved() {
        static bool isMiddleMouseHeld = false;
        static sf::Vector2i lastMousePos;

        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
            if (!isMiddleMouseHeld) {
                isMiddleMouseHeld = true;
                window.setMouseCursorVisible(false);
                lastMousePos = sf::Mouse::getPosition(window);
            }

            sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
            sf::Vector2i mouseDelta = currentMousePos - lastMousePos;

            if (mouseDelta != sf::Vector2i(0, 0)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
                    float sensitivity = 0.01f;
                    getCamera().move(Vector3(
                        sensitivity * static_cast<float>(mouseDelta.x),
                        sensitivity * static_cast<float>(mouseDelta.y / 2),
                        0.0f
                    ));
                } else {
                    float sensitivity = 1.0f;
                    if (inertiaEnabled) {
                    yawVelocity += sensitivity * static_cast<float>(mouseDelta.x);
                    pitchVelocity += sensitivity * static_cast<float>(mouseDelta.y);
                    }
                    else {
                        camera.orbit(Vector3(0, 0, 0), sensitivity * static_cast<float>(mouseDelta.x), sensitivity * static_cast<float>(mouseDelta.y));
                    }

                }

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

    void update(float deltaTime) {
        // Apply damping to the velocities
        yawVelocity *= dampingFactor;
        pitchVelocity *= dampingFactor;

        if (std::abs(yawVelocity) > 0.01f || std::abs(pitchVelocity) > 0.01f) {
            camera.orbit(Vector3(0, 0, 0), yawVelocity * deltaTime, pitchVelocity * deltaTime);
        }
    }
};

#endif
