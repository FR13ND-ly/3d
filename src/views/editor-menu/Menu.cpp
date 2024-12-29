#include "Menu.hpp"

#include <iostream>

void Menu::onActivate() {
    // Default implementation: does nothing. Can be overridden by subclasses.
}

void Menu::onDeactivate() {
    // Default implementation: does nothing. Can be overridden by subclasses.
}

void Menu::addComponent(std::shared_ptr<Component> component) {
    components.push_back(component);
}

void Menu::draw(sf::RenderWindow& window) {
    for (const auto& component : components) {
        component->draw(window);
    }
}

void Menu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    std::cout << components.size() << std::endl;
    for (const auto& component : components) {
        component->handleEvent(event, window);
    }
}

void Menu::setParentPosition(const sf::Vector2f& position) {
    parentPosition = position;
}
