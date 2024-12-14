#include "App.hpp"
#include "../../views/ViewsManager.hpp"
#include "../../utils/WindowManager.hpp"

App::App() {}

void App::run() {
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    ViewsManager& viewsManager = ViewsManager::getInstance();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            viewsManager.handleEvent(event, window);
        }

        window.clear(sf::Color::Black);
        viewsManager.draw(window);
        window.display();
    }
}
