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

    virtual void onActivate();    // Called when the view is activated
    virtual void onDeactivate();  // Called when the view is deactivated

    virtual void addComponent(std::shared_ptr<Component> component);  // Add a component to the view
    virtual void draw(sf::RenderWindow& window);  // Draw all components in the view
    virtual void handleEvent(const sf::Event& event, sf::RenderWindow& window);  // Handle events for all components
};

#endif
