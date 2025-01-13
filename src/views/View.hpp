#ifndef VIEW_HPP
#define VIEW_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../core/ui/Component.hpp"

class View {
public:
    std::vector<std::shared_ptr<Component>> components;

    virtual ~View() = default;

    virtual void onActivate();
    virtual void onDeactivate();

    virtual void addComponent(std::shared_ptr<Component> component);
    virtual void draw(sf::RenderWindow& window);
    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window);
};

#endif
