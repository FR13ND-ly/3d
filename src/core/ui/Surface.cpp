#include "Surface.hpp"

// Constructor definition
Surface::Surface(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color) {
    rectangle.setPosition(position);
    rectangle.setSize(size);
    rectangle.setFillColor(color);
}

// Draw function definition
void Surface::draw(sf::RenderWindow& window) {
    window.draw(rectangle);
}

// Handle event function definition
void Surface::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && inBounds(sf::Mouse::getPosition(window))) {
        if (onClick) onClick();
        if (onClickWithBool) onClickWithBool(true);
    } else if (event.type == sf::Event::MouseMoved) {
        if (onHover) onHover(inBounds(sf::Mouse::getPosition(window)));
    }
}

// Setters
void Surface::setColor(const sf::Color& color) {
    rectangle.setFillColor(color);
}

void Surface::setSize(const sf::Vector2f& size) {
    rectangle.setSize(size);
}

// InBounds function definition
bool Surface::inBounds(const sf::Vector2i& mousePos) const {
    return rectangle.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

float Surface::getHeight() const {
    return rectangle.getSize().y;
}

sf::Vector2f Surface::getPosition() const {
    return rectangle.getPosition();
}

void Surface::setPosition(const sf::Vector2f& newPosition) {
    rectangle.setPosition(newPosition);
}