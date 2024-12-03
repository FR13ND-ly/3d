    #ifndef APP_HPP
    #define APP_HPP

    #include <SFML/Graphics.hpp>
    #include "Scene.hpp"
    #include "Renderer.hpp"
    #include "InputHandler.hpp"
    #include "../objects/Cube.hpp"
    #include "../objects/Sphere.hpp"
    #include "../objects/Frustum.hpp"

    class App {
    public:
        App() {
            sf::ContextSettings settings;
            // settings.depthBits = 24; // 24-bit depth buffer for better precision
            // settings.stencilBits = 8; // Optional: Enable stencil buffer
            // settings.antialiasingLevel = 4; // Optional: Enable anti-aliasing

            window.create(sf::VideoMode(800, 600), "3D Editor", sf::Style::Default, settings);

        }

        void run() {
            Scene scene;
            Renderer renderer(window);
            InputHandler inputHandler(window, scene);

            scene.addObject(std::shared_ptr<Object3d>(new Cube(1.f)));
            // scene.addObject(std::shared_ptr<Object3d>(new Cube(1.f)));
            // std::shared_ptr<Object3d> frustum = std::make_shared<Frustum>(90.0f, 800.0f / 600.0f, 1.0f, 100.0f);
            // scene.addObject(frustum);
            sf::Vector2u windowSize = window.getSize();
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
