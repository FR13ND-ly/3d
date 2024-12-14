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
    Button(const sf::Vector2f &position, const sf::Vector2f &size, const std::string &text, const sf::Color &color = sf::Color::White, const sf::Color &textColor = sf::Color::Black);

    void draw(sf::RenderWindow &window);
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window);

protected:
    sf::RectangleShape buttonRect;
    sf::Text buttonText;
    sf::Color color;

    bool inBounds(const sf::Vector2i &mousePos) const;
    sf::Color darkenColor(const sf::Color &color, int amount);
};

#endif
