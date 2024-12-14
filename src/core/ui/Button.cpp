#include "Button.hpp"

Button::Button(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &text, const sf::Color &color, const sf::Color &textColor) {
    buttonRect.setSize(size);
    buttonRect.setPosition(position);
    buttonRect.setFillColor(color);
    // buttonRect.setOutlineColor(outlineColor);
    // buttonRect.setOutlineThickness(2);

    const sf::Font &font = FontManager::getInstance().getFont();
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(static_cast<unsigned int>(size.y * 0.5f));
    buttonText.setFillColor(textColor);

    this->color = color;

    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    buttonText.setPosition(
        position.x + size.x / 2,
        position.y + size.y / 2 - textBounds.height / 2
    );
}

void Button::draw(sf::RenderWindow &window) {
    window.draw(buttonRect);
    window.draw(buttonText);
}

void Button::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    bool isHovered = inBounds(mousePos);

    if (isHovered) {
        buttonRect.setFillColor(darkenColor(color, 40));
        if (onHover) {
            onHover(true);
        }
    } else {
        buttonRect.setFillColor(color);
        if (onHover) {
            onHover(false);
        }
    }

    if (isHovered && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        buttonRect.setFillColor(darkenColor(color, 60));
        if (onClick) {
            onClick();
        }
    }
}

bool Button::inBounds(const sf::Vector2i &mousePos) const {
    return buttonRect.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

sf::Color Button::darkenColor(const sf::Color &color, int amount) {
    return sf::Color(
        std::max(0, color.r - amount),
        std::max(0, color.g - amount),
        std::max(0, color.b - amount)
    );
}