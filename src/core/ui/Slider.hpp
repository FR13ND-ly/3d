#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <SFML/Graphics.hpp>
#include "Component.hpp"

class Slider : public Component {
public:
    // Constructor
    Slider(const sf::Vector2f& position, const sf::Vector2f& size, float minValue, float maxValue, float currentValue, const sf::Color& trackColor = sf::Color::White, const sf::Color& handleColor = sf::Color::Red);

    // Draw and handle event functions
    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;

    // Getter and Setter for value
    float getValue() const;
    void setValue(float value);

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
