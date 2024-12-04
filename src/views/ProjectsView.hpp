#ifndef PROJECTS_VIEW_HPP
#define PROJECTS_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "../core/ui/Component.hpp"
#include "../core/ui/Text.hpp"
#include "../core/ui/Button.hpp"
#include "View.hpp"
#include "viewSwitcher.hpp"

class ProjectsView: public View {
public:
    ProjectsView() {
        auto myButton = std::make_shared<Button>(
        sf::Vector2f(100, 100),
            sf::Vector2f(200, 50),
            "Click Me",
            sf::Color::Red
        );

        myButton->setOnClick([]() {
            switchToView("editor");
        });

        auto myText = std::make_shared<Text>(
            sf::Vector2f(400, 100),
            20,
            "Projects Page"
        );
        auto myText2 = std::make_shared<Text>(
            sf::Vector2f(400, 200),
            20,
            "Salut Robert"
        );

        this->addComponent(myText);
        this->addComponent(myText2);
        this->addComponent(std::dynamic_pointer_cast<Component>(myButton));
    }
    std::vector<std::shared_ptr<Component>> components;

    virtual void onActivate() {}

    virtual void onDeactivate() {}
};

#endif
