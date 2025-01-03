#include "Text.hpp"

// Constructor definition
Text::Text(const sf::Vector2f &position, const int size, const std::string &text, const sf::Color &color)
    : color(color) {
    const sf::Font &font = FontManager::getInstance().getFont();
    content.setFont(font);
    content.setPosition(position);
    content.setString(text);
    content.setCharacterSize(size);
    content.setFillColor(color);
}

// Draw function definition
void Text::draw(sf::RenderWindow &window) {
    window.draw(content);
}

// Handle event (currently not doing anything)
void Text::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {}


void Text::setBold() {
    content.setStyle(sf::Text::Bold);
}

// Set new text content
void Text::setText(const std::string &text) {
    content.setString(text);
}

// InBounds function definition (for mouse interaction)
bool Text::inBounds(const sf::Vector2i &mousePos) const {
    return content.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

float Text::getHeight() const {
    return content.getGlobalBounds().height;
}

float Text::getWidth() const {
    return content.getGlobalBounds().width;
}

sf::Vector2f Text::getPosition() const {
    return content.getPosition();
}

void Text::setPosition(const sf::Vector2f& newPosition) {
    content.setPosition(newPosition);
}