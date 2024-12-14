#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <SFML/Graphics.hpp>
#include "Component.hpp"

class Surface : public Component {
public:
    Surface(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& color = sf::Color::White);

    void draw(sf::RenderWindow& window) override;

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    void setColor(const sf::Color& color);
    void setSize(const sf::Vector2f& size);
    void setPosition(const sf::Vector2f& position);

protected:
    sf::RectangleShape rectangle;

    bool inBounds(const sf::Vector2i& mousePos) const override;
};

#endif