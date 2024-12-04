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

    WindowManager() {
        // sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();
        window.create(sf::VideoMode(800, 600), "3D Editor");
    }

    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
};

#endif
