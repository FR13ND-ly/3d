#ifndef HOME_VIEW_HPP
#define HOME_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "View.hpp"
#include "../core/ui/Component.hpp"
#include "../core/ui/Button.hpp"
#include "../core/ui/Text.hpp"
#include "viewSwitcher.hpp"

class HomeView : public View {
public:
    HomeView() {
        auto myButton = std::make_shared<Button>(
            sf::Vector2f(100, 100),
            sf::Vector2f(200, 50),
            "Click Me"
        );

        myButton->setOnClick([this]() {
            switchToView("projects");
        });

        auto myText = std::make_shared<Text>(
            sf::Vector2f(400, 100),
            20,
            "Home Page"
        );

        this->addComponent(myText);
        this->addComponent(myButton);
    }
};

#endif
