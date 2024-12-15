#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "../../utils/FontManager.hpp"
#include <string>

class Input : public Component {
public:
    Input(const sf::Vector2f& position, const sf::Vector2f& size,
          const std::string& titleText = "",
          const std::string& placeholder = "Enter text",
          const sf::Color& boxColor = sf::Color::White,
          const sf::Color& textColor = sf::Color::Black);

    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    std::string getValue() const;
    void setValue(const std::string& value);

    float getHeight() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

protected:
    sf::RectangleShape inputBox;
    sf::Text inputText;
    sf::Text placeholderText;
    sf::Text title;

    sf::Color boxColor;
    sf::Color textColor;

    std::string currentText;
    bool isFocused = false;

    bool inBounds(const sf::Vector2i& mousePos) const;
};

#endif
