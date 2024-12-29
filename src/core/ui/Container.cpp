// Container.cpp
#include "Container.hpp"

#include <iostream>

Container::Container(const sf::Vector2f& position, const sf::Vector2f& size)
    : position(position), size(size) {
    background.setSize(size);
    background.setPosition(position);
    background.setFillColor(sf::Color(50, 50, 50)); // Default background color
}

void Container::addComponent(std::shared_ptr<Component> component) {
    components.push_back(component);
    updateComponentPositions();
}

void Container::clearComponents() {
    components.clear();
}

std::vector<std::shared_ptr<Component>> Container::getComponents() const {
    return components;
}

void Container::updateComponentPositions() {
    float currentY = position.y;

    for (auto& component : components) {
        // If the component's Y position is unchanged, stack it vertically
        if (component->getPosition().y == 0) {
            component->setPosition({component->getPosition().x, currentY});
            currentY += component->getHeight(); // Move to the next position
        } else {
            // Otherwise, respect the manually set Y position
            currentY = component->getPosition().y + component->getHeight();
        }
    }
}

void Container::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    for (auto& component : components) {
        if (component.get() == reinterpret_cast<Component*>(0xfeeefeeefeeefeee)) {
            std::cerr << "Component pointer has been corrupted!" << std::endl;
            continue;
        }
        try {
            component->handleEvent(event, window);
        } catch (const std::exception& e) {
            std::cerr << "Exception caught in handleEvent: " << e.what() << std::endl;
        }
    }
}

void Container::draw(sf::RenderWindow& window) {
    window.draw(background);

    for (const auto& component : components) {
        component->draw(window);
    }
}

bool Container::inBounds(const sf::Vector2i &mousePos) const {
    sf::FloatRect bounds(position.x, position.y, size.x, size.y);
    return bounds.contains(mousePos.x, mousePos.y);
}

float Container::getHeight() const {
    return background.getGlobalBounds().height;
}

sf::Vector2f Container::getPosition() const {
    return position;
}

void Container::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
    background.setPosition(newPosition);
    updateComponentPositions();
}
