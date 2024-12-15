#include "Input.hpp"
#include <iostream>
#include <algorithm>

Input::Input(const sf::Vector2f& position, const sf::Vector2f& size,
             const std::string& titleText, const std::string& placeholder,
             const sf::Color& boxColor, const sf::Color& textColor)
    : boxColor(boxColor), textColor(textColor), currentText(""), isFocused(false) {

    inputBox.setSize(size);
    inputBox.setPosition(position);
    inputBox.setFillColor(boxColor);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1.f);

    inputText.setFont(FontManager::getInstance().getFont());
    inputText.setCharacterSize(static_cast<unsigned int>(size.y * 0.5f));
    inputText.setFillColor(sf::Color(100, 100, 100));
    inputText.setPosition(position.x + 5, position.y + (size.y * 0.25f));

    placeholderText.setFont(FontManager::getInstance().getFont());
    placeholderText.setCharacterSize(static_cast<unsigned int>(size.y * 0.5f));
    placeholderText.setFillColor(sf::Color(150, 150, 150));
    placeholderText.setString(placeholder);
    placeholderText.setPosition(position.x + 5, position.y + (size.y * 0.25f));

    title.setFont(FontManager::getInstance().getFont());
    title.setCharacterSize(static_cast<unsigned int>(size.y * 0.25f));
    title.setFillColor(textColor);
    title.setString(titleText);
    title.setPosition(position.x + 5, position.y + size.y * 0.05f);
}

void Input::draw(sf::RenderWindow& window) {
    window.draw(inputBox);
    window.draw(title);

    if (currentText.empty() && !isFocused) {
        window.draw(placeholderText);
    } else {
        inputText.setString(currentText);
        window.draw(inputText);
    }
}

void Input::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        isFocused = inBounds(sf::Mouse::getPosition(window));
    }
    if (isFocused) {
        inputText.setFillColor(sf::Color::Black);
    }

    if (isFocused && event.type == sf::Event::TextEntered) {
        char enteredChar = static_cast<char>(event.text.unicode);

        if (enteredChar == 8) { // Backspace
            if (!currentText.empty()) {
                currentText.pop_back();
            }
        } else if (enteredChar >= 32 && enteredChar <= 126) { // Printable characters
            currentText += enteredChar;
        }
        if (onClickWithString) {
            onClickWithString(currentText);
        }
    }
}

std::string Input::getValue() const {
    return currentText;
}

void Input::setValue(const std::string& value) {
    currentText = value;
}

sf::Vector2f Input::getPosition() const {
    return inputBox.getPosition();
}

void Input::setPosition(const sf::Vector2f& position) {
    inputBox.setPosition(position);
    inputText.setPosition(position.x + 5, position.y + (inputBox.getSize().y * 0.25f));
    placeholderText.setPosition(position.x + 5, position.y + (inputBox.getSize().y * 0.25f));
    title.setPosition(position.x + 5, position.y - inputBox.getSize().y * 0.6f);
}

float Input::getHeight() const {
    return inputBox.getSize().y;
}


bool Input::inBounds(const sf::Vector2i& mousePos) const {
    return inputBox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}