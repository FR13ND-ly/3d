#include "View.hpp"

void View::onActivate() {
    // Default implementation: does nothing. Can be overridden by subclasses.
}

void View::onDeactivate() {
    // Default implementation: does nothing. Can be overridden by subclasses.
}

void View::addComponent(std::shared_ptr<Component> component) {
    components.push_back(component);
}

void View::draw(sf::RenderWindow& window) {
    for (const auto& component : components) {
        component->draw(window);
    }
}

void View::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    for (const auto& component : components) {
        component->handleEvent(event, window);
    }
}
