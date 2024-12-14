#include "InputHandler.hpp"

InputHandler::InputHandler(sf::RenderWindow& window, ViewsManager& viewsManager)
    : window(window), viewsManager(viewsManager) {}

void InputHandler::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        handleKeyPressed(event.key.code);
    }

    if (event.type == sf::Event::MouseMoved) {
        handleMouseMoved();
    }
}

void InputHandler::handleKeyPressed(sf::Keyboard::Key key) {
    // auto& currentView = viewsManager.getCurrentView();
    // if (!currentView) return;
    //
    // Vector3 direction;
    // auto& scene = currentView->getScene();
    // auto& camera = scene.getCamera();
    //
    // switch (key) {
    //     case sf::Keyboard::W:
    //         direction = Vector3(0, 0, 0.1f);
    //         break;
    //     case sf::Keyboard::S:
    //         direction = Vector3(0, 0, -0.1f);
    //         break;
    //     case sf::Keyboard::A:
    //         direction = Vector3(-0.1f, 0, 0);
    //         break;
    //     case sf::Keyboard::D:
    //         direction = Vector3(0.1f, 0, 0);
    //         break;
    //     case sf::Keyboard::Q:
    //         direction = Vector3(0, 0.1f, 0);
    //         break;
    //     case sf::Keyboard::E:
    //         direction = Vector3(0, -0.1f, 0);
    //         break;
    //     default:
    //         break;
    // }

    // if (direction != Vector3(0, 0, 0)) {
    //     camera.move(direction);
    // }
}

void InputHandler::handleMouseMoved() {
    // static bool isMiddleMouseHeld = false;
    //
    // auto& currentView = viewsManager.getCurrentView();
    // if (!currentView) return;
    //
    // auto& scene = currentView->getScene();
    // auto& camera = scene.getCamera();
    //
    // if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
    //     if (!isMiddleMouseHeld) {
    //         isMiddleMouseHeld = true;
    //         window.setMouseCursorVisible(false);
    //     }
    //
    //     sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
    //     sf::Vector2i currentMousePos = sf::Mouse::getPosition(window);
    //     sf::Vector2i mouseDelta = currentMousePos - windowCenter;
    //
    //     if (mouseDelta != sf::Vector2i(0, 0)) {
    //         float sensitivity = 0.1f;
    //         camera.rotateYaw(sensitivity * static_cast<float>(mouseDelta.x));
    //         camera.rotatePitch(sensitivity * static_cast<float>(mouseDelta.y));
    //         sf::Mouse::setPosition(windowCenter, window);
    //     }
    // } else {
    //     if (isMiddleMouseHeld) {
    //         isMiddleMouseHeld = false;
    //         window.setMouseCursorVisible(true);
    //     }
    // }
}
