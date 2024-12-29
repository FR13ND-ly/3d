#include "App.hpp"
#include "../../views/ViewsManager.hpp"
#include "../../utils/WindowManager.hpp"
#include "../ui/Snackbar.hpp"

App::App() {}

void App::run() {
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    ViewsManager& viewsManager = ViewsManager::getInstance();
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed || event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
            viewsManager.handleEvent(event, window);
        }
        window.clear(sf::Color::Black);
        viewsManager.draw(window);
        Snackbar::getInstance().update();
        Snackbar::getInstance().draw(window);
        window.display();
    }
}
