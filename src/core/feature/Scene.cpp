#include "Scene.hpp"
#include <memory>
#include "../objects/ObjectsFactory.hpp"

Scene::Scene(sf::RenderWindow &window)
    : camera(window.getSize().x, window.getSize().y, 90.0f, 0.1f, 100.0f),
      renderer(window),
      window(window),
      yawVelocity(0.0f),
      pitchVelocity(0.0f),
      dampingFactor(0.95f) {
    addObject(std::shared_ptr<Object3d>(new GridPlane()));
}

Scene& Scene::getInstance(sf::RenderWindow& window) {
    static Scene instance(window);
    return instance;
}

void Scene::addObject(std::shared_ptr<Object3d> object) {
    objects.push_back(object);
    onChangeSelectedObjectIndex(objects.size() - 1);
}

void Scene::addObject(const std::string& objectType, float param1, int param2) {
    ObjectsFactory factory;

    std::shared_ptr<Object3d> object = factory.createObject(objectType, param1, param2);
    addObject(object);
}

const std::vector<std::shared_ptr<Object3d>>& Scene::getObjects() const {
    return objects;
}

void Scene::setCamera(const Camera &camera) {
    // this->camera = camera; // Optional if you wish to set the camera manually
}

const Camera& Scene::getCamera() const {
    return camera;
}

Camera& Scene::getCamera() {
    return camera;
}

void Scene::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (event.type == sf::Event::KeyPressed) {
        handleKeyPressed(event.key.code);
    }
    if (!inBounds(sf::Mouse::getPosition(window))) return;
    if (event.type == sf::Event::MouseMoved) {
        handleMouseMoved();
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        handleScroll(event.mouseWheelScroll);
    }
}

void Scene::draw(sf::RenderWindow &window) {
    if (Config::getInstance().getInertia()) {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();
        update(deltaTime);
    }
    renderer.render(objects, camera);
}

void Scene::onChangeSelectedObjectIndex(int selectedObjectIndex) {
    for (int i = 0; i < objects.size(); i++) {
        objects[i]->isSelected = false;
    }
    this->selectedObjectIndex = selectedObjectIndex;
    objects[selectedObjectIndex]->isSelected = true;
}

void Scene::handleScroll(sf::Event::MouseWheelScrollEvent event) {
    camera.zoom(Config::getInstance().getSensitivity() * 0.5f * event.delta);
}

void Scene::handleKeyPressed(sf::Keyboard::Key key) {
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
            camera.rotatePitch(Config::getInstance().getSensitivity() * 1.5f);
            break;
        case sf::Keyboard::S:
            camera.rotatePitch(Config::getInstance().getSensitivity() * -1.5f);
            break;
        case sf::Keyboard::A:
            camera.rotateYaw(Config::getInstance().getSensitivity() * -1.5f);
            break;
        case sf::Keyboard::D:
            camera.rotateYaw(Config::getInstance().getSensitivity() * 1.5f);
            break;
        case sf::Keyboard::Q:
            getCamera().move(Vector3(0.0f, Config::getInstance().getSensitivity() * 0.1f, 0.0f));
        break;
        case sf::Keyboard::E:
            getCamera().move(Vector3(0.0f, -Config::getInstance().getSensitivity() * 0.1f, 0.0f));
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
            object->translate(Vector3(-0.05f, 0.0f, 0.0f));  // Translate left
            break;
        case sf::Keyboard::Right:
            object->translate(Vector3(0.05f, 0.0f, 0.0f));  // Translate right
            break;
        case sf::Keyboard::Up:
            object->translate(Vector3(0.0f, -0.05f, 0.0f));  // Translate up
            break;
        case sf::Keyboard::Down:
            object->translate(Vector3(0.0f, 0.05f, 0.0f));  // Translate down
            break;
        default:
            break;
    }
    getCamera().move(direction);
}

void Scene::handleMouseMoved() {
    static bool isMiddleMouseHeld = false;
    static sf::Vector2i lastMousePos;

    // Get the center of the window
    sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);

    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
        if (!isMiddleMouseHeld) {
            isMiddleMouseHeld = true;
            window.setMouseCursorVisible(false);
            // Set the mouse position to the center initially
            sf::Mouse::setPosition(windowCenter, window);
            lastMousePos = windowCenter;
        }

        sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
        sf::Vector2i mouseDelta = currentMousePos - lastMousePos;

        if (mouseDelta != sf::Vector2i(0, 0)) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::RShift)) {
                float sensitivity = Config::getInstance().getSensitivity() * 0.01f;
                getCamera().move(Vector3(
                    sensitivity * static_cast<float>(-mouseDelta.x),
                    sensitivity * static_cast<float>(mouseDelta.y / 2),
                    0.0f
                ));
            } else {
                float sensitivity = Config::getInstance().getSensitivity() * 0.5f;
                if (Config::getInstance().getInertia()) {
                    yawVelocity += sensitivity * static_cast<float>(mouseDelta.x);
                    pitchVelocity += sensitivity * static_cast<float>(mouseDelta.y);
                }
                else {
                    camera.orbit(Vector3(0, 0, 0), 0.1 * sensitivity * static_cast<float>(mouseDelta.x), 0.1 * sensitivity * static_cast<float>(mouseDelta.y));
                }
            }

            // Reset the mouse position to the center
            sf::Mouse::setPosition(windowCenter, window);
            lastMousePos = windowCenter;
        }
    } else {
        if (isMiddleMouseHeld) {
            isMiddleMouseHeld = false;
            window.setMouseCursorVisible(true);
        }
    }
}


void Scene::update(float deltaTime) {
    yawVelocity *= dampingFactor;
    pitchVelocity *= dampingFactor;

    if (std::abs(yawVelocity) > 0.01f || std::abs(pitchVelocity) > 0.01f) {
        camera.orbit(Vector3(0, 0, 0), yawVelocity * deltaTime, pitchVelocity * deltaTime);
    }
}

int Scene::getSelectedObjectIndex() const {
    return selectedObjectIndex;
}

bool Scene::inBounds(const sf::Vector2i &mousePos) const {
    return mousePos.x >= 0 && mousePos.x < window.getSize().x - 350 && mousePos.y >= 0 && mousePos.y < window.getSize().y;
}

float Scene::getHeight() const {
    return window.getSize().y;
}

sf::Vector2f Scene::getPosition() const {
    return sf::Vector2f({0.f, 0.f});
}

void Scene::setPosition(const sf::Vector2f& position) {}

bool Scene::getVerticesEditMode() {
    return verticesEditMode;
}

void Scene::setVerticesEditMode(bool value) {
    verticesEditMode = value;
}
