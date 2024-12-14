#include "CheckBox.hpp"

CheckBox::CheckBox(const sf::Vector2f& position, float size, bool isChecked, const sf::Color& boxColor, const sf::Color& checkColor)
    : isChecked(isChecked), size(size), boxColor(boxColor), checkColor(checkColor) {

    box.setSize(sf::Vector2f(size, size));
    box.setPosition(position);
    box.setFillColor(boxColor);
    box.setOutlineColor(sf::Color::Black);
    box.setOutlineThickness(2);

    check.setSize(sf::Vector2f(size * 0.6f, size * 0.6f));
    check.setPosition(position.x + size * 0.2f, position.y + size * 0.2f);
    check.setFillColor(checkColor);
    check.setOutlineThickness(0);
}

void CheckBox::draw(sf::RenderWindow& window) {
    window.draw(box);
    if (isChecked) {
        window.draw(check);
    }
}

void CheckBox::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (inBounds(mousePos)) {
            isChecked = !isChecked;
            if (onClickWithBool) {
                onClickWithBool(isChecked);  // Pass the current state (checked or unchecked)
            }
        }
    }
}

bool CheckBox::getChecked() const {
    return isChecked;
}

void CheckBox::setChecked(bool checked) {
    isChecked = checked;
}

bool CheckBox::inBounds(const sf::Vector2i& mousePos) const {
    return box.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}