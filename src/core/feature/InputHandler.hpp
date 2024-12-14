#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "../objects/Object3d.hpp"
#include "../../utils/Matrix4.hpp"
#include "../../views/ViewsManager.hpp"

class InputHandler {
public:
    InputHandler(sf::RenderWindow& window, ViewsManager& viewsManager);

    void handleEvent(const sf::Event& event);

private:
    sf::RenderWindow& window;
    ViewsManager& viewsManager;

    void handleKeyPressed(sf::Keyboard::Key key);
    void handleMouseMoved();
};

#endif
