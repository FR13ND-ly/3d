#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"
#include "../objects/Cube.hpp"
#include "../objects/Sphere.hpp"

class App {
public:
    App() : window(sf::VideoMode(800, 600), "3D Editor") {}

    void run() {
        Scene scene;
        Renderer renderer(window);
        InputHandler inputHandler(window, scene);

        scene.addObject(new Cube(1.f));
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();

                inputHandler.handleEvent(event);
            }
            window.clear(sf::Color::Black);
            renderer.render(scene);
            window.display();
        }
    }

private:
    sf::RenderWindow window;
};

#endif
