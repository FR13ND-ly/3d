#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>

class App {
public:
    App();
    void run();

private:
    sf::RenderWindow window;
};

#endif
