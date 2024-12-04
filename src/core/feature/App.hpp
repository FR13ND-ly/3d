#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include "../../views/ViewsManager.hpp"
#include "../../utils/WindowManager.hpp"
#include "Scene.hpp"
#include "InputHandler.hpp"
#include "../objects/Cube.hpp"
#include "../objects/Sphere.hpp"
#include <iostream>

class App {
public:
    App() {}

    void run() {
        sf::RenderWindow& window = const_cast<sf::RenderWindow&>(WindowManager::getInstance().getWindow());
        ViewsManager& viewsManager = ViewsManager::getInstance();
        InputHandler inputHandler(window, viewsManager);
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                viewsManager.handleEvent(event, window);
                // inputHandler.handleEvent(event);
            }
            window.clear(sf::Color::Black);
            viewsManager.draw(window);
            window.display();
        }
    }

private:
    sf::RenderWindow window;
};

#endif
