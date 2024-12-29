#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <SFML/Graphics.hpp>
#include "Component.hpp"

class Slider : public Component {
public:
    Slider(const sf::Vector2f& position, const sf::Vector2f& size, float minValue, float maxValue, float currentValue, const sf::Color& trackColor = sf::Color::White, const sf::Color& handleColor = sf::Color::Yellow);

    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;

    float getValue() const;
    void setValue(float value);

    float getHeight() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

protected:
    // Member variables
    sf::RectangleShape track;
    sf::RectangleShape handle;
    float minValue;
    float maxValue;
    float currentValue;
    bool isDragging;
    sf::Vector2f position;
    sf::Vector2f size;
};

#endif // SLIDER_HPP
