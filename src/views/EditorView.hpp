#ifndef EDITOR_VIEW_HPP
#define EDITOR_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "View.hpp"
#include "../core/ui/Component.hpp"
#include "../core/ui/Button.hpp"
#include "../core/ui/Text.hpp"
#include "../core/feature/Scene.hpp"
#include "../utils/WindowManager.hpp"
#include "viewSwitcher.hpp"
#include "../core/objects/Cube.hpp"
#include "../core/objects/Sphere.hpp"

class EditorView : public View {
public:
    EditorView() {
        initializeScene();
        createUI();
    }
    void clearComponents() {
        components.clear();
    }

private:
    std::shared_ptr<Scene> scene;

    void initializeScene() {
        sf::RenderWindow& window = const_cast<sf::RenderWindow&>(WindowManager::getInstance().getWindow());
        scene = std::make_shared<Scene>(window);

        this->addComponent(scene);
    }

    void createUI() {
        auto myButton = std::make_shared<Button>(
            sf::Vector2f(50, 100),
            sf::Vector2f(200, 50),
            "Home"
        );
        auto Reset = std::make_shared<Button>(
            sf::Vector2f(50, 400),
            sf::Vector2f(200, 50),
            "Reset"
        );

        auto sphereButton = std::make_shared<Button>(
            sf::Vector2f(50, 300),
            sf::Vector2f(200, 50),
            "Sphere"
        );
        auto cubeButton = std::make_shared<Button>(
            sf::Vector2f(50, 200),
               sf::Vector2f(200, 50),
                "Cube"
        );
        cubeButton->setOnClick([this]() {
           this->scene->addObject(std::shared_ptr<Object3d>(new Cube(1.f)));
        });

        sphereButton->setOnClick([this]() {
            this->scene->addObject(std::shared_ptr<Object3d>(new Sphere(1.f, 16, 16)));
        });

        myButton->setOnClick([this]() {
            this->clearComponents();
            initializeScene();
            createUI();
            switchToView("home");
        });
        Reset->setOnClick([this]() {
            this->clearComponents();
            initializeScene();

            createUI();
        });

        this->addComponent(myButton);
        this->addComponent(Reset);
        this->addComponent(sphereButton);
        this->addComponent(cubeButton);
    }
};

#endif
