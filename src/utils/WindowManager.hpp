#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class WindowManager {
public:
    static WindowManager& getInstance() {
        static WindowManager instance;
        return instance;
    }

    const sf::RenderWindow& getWindow() const {
        return window;
    }

private:
    sf::RenderWindow window;

    enum State
    {
        Windowed,
        Minimized,
        Maximized,
        Fullscreen
    };


    WindowManager() {
        sf::ContextSettings windowSettings;
        windowSettings.antialiasingLevel = 8;
        sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
        window.create(desktopMode, "3D Editor", sf::Style::Close, sf::ContextSettings(windowSettings.antialiasingLevel));
        window.setVerticalSyncEnabled(true);
        window.setSize(sf::Vector2u(desktopMode.width - 50, desktopMode.height - 100));
    }

    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
};

#endif
