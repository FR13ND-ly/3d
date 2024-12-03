#ifndef APP_HPP
#define APP_HPP

#include <SFML/Graphics.hpp>
#include "Scene.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"
#include "../objects/Cube.hpp"
#include "../ui/Button.hpp"
#include "../objects/Sphere.hpp"
#include <iostream>
#include "../../views/ViewsManager.hpp"


class App {
public:
    App() {
        window.create(sf::VideoMode(800, 600), "3D Editor");
    }

    void run() {
        Scene scene;
        Renderer renderer(window);
        InputHandler inputHandler(window, scene);
        Button myButton(sf::Vector2f(100, 100), sf::Vector2f(200, 50), "Click Me");
        myButton.setOnClick([]() {
            std::cout << "Hello World" << std::endl;
        });
        ViewManager viewManager = ViewManager();
        // scene.addObject(std::shared_ptr<Object3d>(new Cube(1.f)));
        // scene.addObject(std::shared_ptr<Object3d>(new Sphere(1.f, 16, 16)));
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
                myButton.handleEvent(event, window);
                inputHandler.handleEvent(event);
            }
            window.clear(sf::Color::Black);
            renderer.render(scene);
            viewManager.draw(window);
            window.display();
        }
    }

private:
    sf::RenderWindow window;
};

#endif
