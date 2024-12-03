#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include "../../utils/FontManager.hpp"
#include "Component.hpp"

struct ButtonSettings {
    sf::Vector2f pos;
    sf::Vector2f size;
    std::string text;
    sf::Color color;
};

class Button: public Component {
public:
    Button(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &text, const sf::Color &color = sf::Color::White, const sf::Color &outlineColor = sf::Color::Black) {
        buttonRect.setSize(size);
        buttonRect.setPosition(position);
        buttonRect.setFillColor(color);
        buttonRect.setOutlineColor(outlineColor);
        buttonRect.setOutlineThickness(2);

        const sf::Font &font = FontManager::getInstance().getFont();
        buttonText.setFont(font);
        buttonText.setString(text);
        buttonText.setCharacterSize(static_cast<unsigned int>(size.y * 0.5f));
        buttonText.setFillColor(sf::Color::Black);

        sf::FloatRect textBounds = buttonText.getLocalBounds();
        buttonText.setOrigin(textBounds.width / 2, textBounds.height / 2);
        buttonText.setPosition(
            position.x + size.x / 2,
            position.y + size.y / 2 - textBounds.height / 2
        );
    }

    void setOnClick(const std::function<void()> &callback) {
        onClick = callback;
    }

    void draw(sf::RenderWindow &window) {
        window.draw(buttonRect);
        window.draw(buttonText);
    }

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        bool isHovered = inBounds(mousePos);

        if (isHovered) {
            buttonRect.setFillColor(hoverColor(color));
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
            if (onClick) {
                onClick();
            }
        }
    }

protected:
    sf::RectangleShape buttonRect;
    sf::Text buttonText;
    sf::Color color = sf::Color::White;

    bool inBounds(const sf::Vector2i &mousePos) const {
        return buttonRect.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }

    sf::Color hoverColor(const sf::Color &color) {
        return sf::Color(
            std::max(0, color.r - 40),
            std::max(0, color.g - 40),
            std::max(0, color.b - 40)
        );
    }
};

#endif
