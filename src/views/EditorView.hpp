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

class EditorView : public View {
public:
    EditorView() {
        sf::RenderWindow& window = const_cast<sf::RenderWindow&>(WindowManager::getInstance().getWindow());
        auto scene = std::make_shared<Scene>(window);
        scene->addObject(std::shared_ptr<Object3d>(new Cube(1.f)));

        auto myButton = std::make_shared<Button>(
            sf::Vector2f(100, 100),
            sf::Vector2f(200, 50),
            "Home"
        );

        myButton->setOnClick([this]() {
            switchToView("home");
        });

        this->addComponent(scene);
        this->addComponent(myButton);
    }
};

#endif
