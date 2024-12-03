#ifndef VIEW_HPP
#define VIEW_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "../core/ui/Component.hpp"

class View {
public:
    std::vector<std::shared_ptr<Component>> components;

    virtual ~View() = default;

    virtual void onActivate() {}

    virtual void onDeactivate() {}

    virtual void addComponent(std::shared_ptr<Component> component) {
        components.push_back(component);
    }

    void draw(sf::RenderWindow& window) {
        for (const auto& component : components) {
            component->draw(window);
        }
    };

    void handleEvent(const sf::Event& event, const sf::RenderWindow &window) {
        for (const auto& component : components) {
            component->handleEvent(event, window);
        }
    }
};

#endif
