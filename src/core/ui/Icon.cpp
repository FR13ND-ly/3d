#include "Icon.hpp"

Icon::Icon(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& iconName)
    : iconName(iconName) {
    if (!texture.loadFromFile("../src/assets/images/icons/" + iconName + ".png")) {
        throw std::runtime_error("Failed to load icon: " + iconName);
    }
    
    sprite.setTexture(texture);
    sprite.setPosition(position);

    sf::Vector2u textureSize = texture.getSize();
    sprite.setScale(
        size.x / textureSize.x,
        size.y / textureSize.y
    );
}

void Icon::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

void Icon::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed && inBounds(sf::Mouse::getPosition(window))) {
        if (onClick) onClick();
        if (onClickWithBool) onClickWithBool(true);
        if (onClickWithString) onClickWithString(iconName);
    } else if (event.type == sf::Event::MouseMoved) {
        if (inBounds(sf::Mouse::getPosition(window))) {
            if (onHover) onHover();
        } else {
            if (onHoverOut) onHoverOut();
        }
    }
}

bool Icon::inBounds(const sf::Vector2i& mousePos) const {
    return sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

float Icon::getHeight() const {
    return sprite.getGlobalBounds().height;
}

sf::Vector2f Icon::getPosition() const {
    return sprite.getPosition();
}

void Icon::setPosition(const sf::Vector2f& position) {
    sprite.setPosition(position);
}

void Icon::setIconName(const std::string& newIconName) {
    if (newIconName != iconName) {
        iconName = newIconName;
        if (!texture.loadFromFile(iconName)) {
            throw std::runtime_error("Failed to load icon: " + iconName);
        }
        sprite.setTexture(texture);
    }
}

void Icon::setSize(const sf::Vector2f& size) {
    sf::Vector2u textureSize = texture.getSize();
    sprite.setScale(
        size.x / textureSize.x,
        size.y / textureSize.y
    );
}