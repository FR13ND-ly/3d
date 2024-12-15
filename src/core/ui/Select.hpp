#ifndef SELECT_HPP
#define SELECT_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <functional>
#include "Component.hpp"
#include "Button.hpp"

class Select : public Component {
public:
    // Constructor
    Select(const sf::Vector2f& position, const sf::Vector2f& size, const std::vector<std::string>& options, int defaultIndex = 0);

    // Event handling and drawing functions
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;
    void draw(sf::RenderWindow& window) override;

    // Getter functions
    std::string getSelectedOption() const;
    int getSelectedIndex() const;

    // Setter for selection callback
    void setOnSelect(const std::function<void(float)>& callback);

    void onSelect(int index);

    float getHeight() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

protected:
    sf::Vector2f position;
    sf::Vector2f size;
    std::vector<std::string> options;
    std::vector<Button> optionTexts;
    int selectedIndex;
    bool isOpen;

    sf::RectangleShape box;
    sf::Text selectedText;
    sf::Font font;
};

#endif // SELECT_HPP
