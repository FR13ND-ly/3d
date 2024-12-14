#include "WindowManager.hpp"

WindowManager& WindowManager::getInstance() {
    static WindowManager instance;
    return instance;
}

sf::RenderWindow& WindowManager::getWindow() {
    return window;
}

WindowManager::WindowManager() {
    sf::ContextSettings windowSettings;
    windowSettings.antialiasingLevel = 8;
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    window.create(desktopMode, "3D Editor", sf::Style::Close, sf::ContextSettings(windowSettings.antialiasingLevel));
    window.setVerticalSyncEnabled(true);
    window.setSize(sf::Vector2u(desktopMode.width, desktopMode.height));
}
