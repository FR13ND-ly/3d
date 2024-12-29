#ifndef NUMBERINPUT_HPP
#define NUMBERINPUT_HPP

#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include "../../utils/FontManager.hpp"
#include <limits>
#include <string>

class NumberInput : public Component {
public:
    NumberInput(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& titleText = "",
                float step = 0.2f,  const sf::Color& boxColor = sf::Color::White,
                const sf::Color& textColor = sf::Color::Black,
                const std::string& placeholder = "0.0",
                float minValue = std::numeric_limits<float>::lowest(),
                float maxValue = std::numeric_limits<float>::max());

    void draw(sf::RenderWindow& window) override;

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    float getValue() const;
    void setValue(float newValue);

    void setPlaceholder(const std::string& placeholder);
    const std::string& getPlaceholder() const;

    void setMinValue(float min);
    void setMaxValue(float max);

    float getHeight() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;
    float lastUpdatedValue = 0.0f;
protected:
    sf::RectangleShape inputBox;
    sf::Text inputText;
    sf::Text placeholderTextDisplay;
    sf::Text title;
    sf::Color boxColor;
    sf::Color textColor;

    std::string placeholderText;

    std::string displayValue = "0.00";
    float numericValue = 0.0f;

    float step = 1.0f;
    float minValue;
    float maxValue;
    bool isFocused = false;

    void validateAndUpdateValue();
    void updateDisplayValue();

    bool inBounds(const sf::Vector2i& mousePos) const override;
    void clampNumericValue();
};

#endif