#ifndef WINDOW_MANAGER_HPP
#define WINDOW_MANAGER_HPP

#include <SFML/Graphics.hpp>

class WindowManager {
public:
    static WindowManager& getInstance();

    sf::RenderWindow& getWindow();  // Return a non-const reference

private:
    sf::RenderWindow window;
    WindowManager();
    WindowManager(const WindowManager&) = delete;
    WindowManager& operator=(const WindowManager&) = delete;
};

#endif // WINDOW_MANAGER_HPP
