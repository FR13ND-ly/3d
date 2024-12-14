#ifndef INPUT_HPP
#define INPUT_HPP

#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "../../utils/FontManager.hpp"

class Input : public Component {
public:
    Input(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& boxColor = sf::Color::White,
          const sf::Color& textColor = sf::Color::Black, const std::string& placeholderText = "Hello World");

    void draw(sf::RenderWindow& window) override;

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;

    const std::string& getText() const;
    void setText(const std::string& newText);

    void setOnlyNumbers(bool enabled);
    bool getOnlyNumbers() const;

    void setPlaceholder(const std::string& placeholder);
    const std::string& getPlaceholder() const;

protected:
    sf::RectangleShape inputBox;
    sf::Text inputText;
    sf::Text placeholderTextDisplay;
    sf::Color boxColor;
    sf::Color textColor;
    std::string text;
    std::string placeholderText;
    bool isFocused = false;
    bool onlyNumbers = false;

    bool inBounds(const sf::Vector2i& mousePos) const override;
};

#endif
