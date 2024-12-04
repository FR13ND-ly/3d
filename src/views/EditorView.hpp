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
    EditorView(): scene(nullptr) {
        sf::RenderWindow& window = const_cast<sf::RenderWindow&>(WindowManager::getInstance().getWindow());
        auto scene = std::make_shared<Scene>(window);
        scene->addObject(std::shared_ptr<Object3d>(new Cube(1.f)));
        this->scene = scene;
        auto myButton = std::make_shared<Button>(
            sf::Vector2f(100, 100),
            sf::Vector2f(200, 50),
            "Home"
        );

        auto sphereButton = std::make_shared<Button>(
            sf::Vector2f(100, 300),
            sf::Vector2f(200, 50),
            "Sphere"
        );

        sphereButton->setOnClick([this]() {
            this->scene->addObject(std::shared_ptr<Object3d>(new Sphere(1.f, 16, 16)));
        });

        myButton->setOnClick([this]() {
            switchToView("home");
        });

        this->addComponent(scene);
        this->addComponent(myButton);
        this->addComponent(sphereButton);
    }
private:
    std::shared_ptr<Scene> scene;
};

#endif
