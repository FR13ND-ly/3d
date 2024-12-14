#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../../core/ui/Component.hpp"

class Menu {
public:
    std::vector<std::shared_ptr<Component>> components;

    virtual ~Menu() = default;

    virtual void onActivate();
    virtual void onDeactivate();

    virtual void addComponent(std::shared_ptr<Component> component);
    virtual void draw(sf::RenderWindow& window);
    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow& window);

    void setParentPosition(const sf::Vector2f& position);

protected:
    sf::Vector2f parentPosition;
};

#endif
