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
    window.create(desktopMode, "3D Editor", sf::Style::Fullscreen, sf::ContextSettings(windowSettings.antialiasingLevel));
    window.setVerticalSyncEnabled(true);
    window.setSize(sf::Vector2u(desktopMode.width, desktopMode.height));
    sf::Image icon;
    icon.loadFromFile("../src/assets/images/favicon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}
