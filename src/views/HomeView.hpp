#ifndef HOME_VIEW_HPP
#define HOME_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "../core/ui/Component.hpp"

class HomeView: public View {
public:
    HomeView() {
        auto myButton = std::make_shared<Button>(
        sf::Vector2f(100, 100),
            sf::Vector2f(200, 50),
            "Click Me"
        );

        myButton->setOnClick([]() {
            std::cout << "Hello World" << std::endl;
        });

        this->addComponent(std::dynamic_pointer_cast<Component>(myButton));
    }
    std::vector<std::shared_ptr<Component>> components;

    virtual void onActivate() {}

    virtual void onDeactivate() {}
};

#endif
