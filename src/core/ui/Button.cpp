#include "Button.hpp"

#include <iostream>

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
    buttonText.setOrigin(textBounds.width / 2, textBounds.height / 2 + textBounds.top);
    buttonText.setPosition(
        position.x + size.x / 2,
        position.y + size.y / 2
    );
}

void Button::draw(sf::RenderWindow &window) {
    window.draw(buttonRect);
    window.draw(buttonText);
    if (hasIcon) {
        window.draw(icon);
    }
}

void Button::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    sf::Vector2i mousePos;
    if (event.type == sf::Event::MouseMoved) {
        mousePos = {event.mouseMove.x, event.mouseMove.y};
    } else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
        mousePos = {event.mouseButton.x, event.mouseButton.y};
    } else {
        return;
    }
    bool isHovered = inBounds(mousePos);

    if (isHovered) {
        buttonRect.setFillColor(darkenColor(color, 40));
        if (onHover) {
            onHover();
        }
    } else {
        buttonRect.setFillColor(color);
        if (onHoverOut) {
            onHoverOut();
        }
    }

    if (isHovered && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        buttonRect.setFillColor(darkenColor(color, 60));
        if (onClick) {
            onClick();
        }
    }
}

void Button::setIcon(std::string iconName) {
    if (!texture.loadFromFile("../src/assets/images/icons/" + iconName + ".png")) {
        throw std::runtime_error("Failed to load icon: " + iconName);
    }
    icon.setTexture(texture);
    hasIcon = true;
    icon.setScale(buttonRect.getSize().y / texture.getSize().y / 2.f, buttonRect.getSize().y / texture.getSize().y / 2.f);
    icon.setPosition(
        buttonRect.getPosition().x + (buttonRect.getSize().y - icon.getGlobalBounds().height) / 2.f,
        buttonRect.getPosition().y + (buttonRect.getSize().y - icon.getGlobalBounds().height) / 2.f
    );
    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonText.setOrigin(0, textBounds.height / 2 + textBounds.top);
    buttonText.setPosition(
        buttonRect.getPosition().x + buttonRect.getSize().y,
        buttonText.getPosition().y
    );
    if (buttonRect.getSize().x < buttonRect.getSize().y * 1.5f + buttonText.getLocalBounds().width && buttonText.getString() != "") {
        buttonRect.setSize({buttonRect.getSize().y * 1.5f + buttonText.getLocalBounds().width, buttonRect.getSize().y});
    }
}

bool Button::inBounds(const sf::Vector2i &mousePos) const {
    return buttonRect.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

float Button::getHeight() const {
    return buttonRect.getSize().y;
}

float Button::getWidth() const {
    return buttonRect.getSize().x;
}

void Button::setColor(sf::Color color) {
    buttonRect.setFillColor(color);
    this->color = color;
}


sf::Color Button::darkenColor(const sf::Color &color, int amount) {
    return sf::Color(
        std::max(0, color.r - amount),
        std::max(0, color.g - amount),
        std::max(0, color.b - amount)
    );
}

sf::Vector2f Button::getPosition() const {
    return buttonRect.getPosition();
}

void Button::setPosition(const sf::Vector2f &position) {
    // Update button rectangle position
    buttonRect.setPosition(position);

    sf::FloatRect textBounds = buttonText.getLocalBounds();

    if (hasIcon) {
        // Align the icon relative to the button rectangle
        icon.setPosition(
            buttonRect.getPosition().x + (buttonRect.getSize().y - icon.getGlobalBounds().height) / 2.f,
            buttonRect.getPosition().y + (buttonRect.getSize().y - icon.getGlobalBounds().height) / 2.f
        );

        // Align the text relative to the icon
        buttonText.setOrigin(0, textBounds.height / 2 + textBounds.top);
        buttonText.setPosition(
            buttonRect.getPosition().x + buttonRect.getSize().y,
            buttonRect.getPosition().y + buttonRect.getSize().y / 2
        );

        // Adjust button size if necessary to fit the icon and text
        if (buttonRect.getSize().x < buttonRect.getSize().y * 1.5f + textBounds.width) {
            buttonRect.setSize({buttonRect.getSize().y * 1.5f + textBounds.width, buttonRect.getSize().y});
        }
    } else {
        // Align text in the center of the button rectangle
        buttonText.setOrigin(textBounds.width / 2, textBounds.height / 2 + textBounds.top);
        buttonText.setPosition(
            position.x + buttonRect.getSize().x / 2,
            position.y + buttonRect.getSize().y / 2
        );
    }
}

