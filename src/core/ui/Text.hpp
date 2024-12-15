#ifndef TEXT_HPP
#define TEXT_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include "../../utils/FontManager.hpp"
#include "Component.hpp"

class Text : public Component {
public:
    // Constructor
    Text(const sf::Vector2f &position, const int size, const std::string &text, const sf::Color &color = sf::Color::White);

    // Draw function
    void draw(sf::RenderWindow &window) override;

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;

    void setText(const std::string &text);

    float getHeight() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

protected:
    sf::Text content;         // SFML Text object
    sf::Color color;          // Color of the text

    bool inBounds(const sf::Vector2i &mousePos) const override;
};

#endif // TEXT_HPP
