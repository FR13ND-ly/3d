#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include "../../utils/FontManager.hpp"
#include "Component.hpp"

class Text: public Component {
public:
    Text(const sf::Vector2f &position, const int size, const std::string &text, const sf::Color &color = sf::Color::White) {
        const sf::Font &font = FontManager::getInstance().getFont();
        content.setFont(font);
        content.setPosition(position);
        content.setString(text);
        content.setCharacterSize(size);
        content.setFillColor(color);
    }

    void draw(sf::RenderWindow &window) {
        window.draw(content);
    }

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) {}

protected:
    sf::RectangleShape rect;
    sf::Text content;
    sf::Color color = sf::Color::White;

    bool inBounds(const sf::Vector2i &mousePos) const {
        return rect.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }
};

#endif
