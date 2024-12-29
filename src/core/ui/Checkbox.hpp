#ifndef CHECKBOX_HPP
#define CHECKBOX_HPP

#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include <functional>

class CheckBox : public Component {
public:
    CheckBox(const sf::Vector2f& position, float size, bool isChecked = false,
             const sf::Color& boxColor = sf::Color::White, const sf::Color& checkColor = sf::Color::Yellow);

    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;

    bool getChecked() const;
    void setChecked(bool checked);

    float getHeight() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

protected:
    sf::RectangleShape box;
    sf::RectangleShape check;
    bool isChecked;
    float size;
    sf::Color boxColor;
    sf::Color checkColor;

    bool inBounds(const sf::Vector2i& mousePos) const override;
};

#endif
