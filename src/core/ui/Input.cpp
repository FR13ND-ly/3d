#include "Input.hpp"

// Constructor definition
Input::Input(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& boxColor,
             const sf::Color& textColor, const std::string& placeholderText)
    : boxColor(boxColor), textColor(textColor), placeholderText(placeholderText) {

    // Initialize input box
    inputBox.setSize(size);
    inputBox.setPosition(position);
    inputBox.setFillColor(boxColor);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(2);

    // Initialize text properties
    const sf::Font& font = FontManager::getInstance().getFont();
    inputText.setFont(font);
    inputText.setCharacterSize(static_cast<unsigned int>(size.y * 0.6f));
    inputText.setFillColor(textColor);
    inputText.setPosition(position.x + 5, position.y + (size.y - inputText.getCharacterSize()) / 2);

    // Initialize placeholder text
    placeholderTextDisplay.setFont(font);
    placeholderTextDisplay.setCharacterSize(inputText.getCharacterSize());
    placeholderTextDisplay.setFillColor(sf::Color(150, 150, 150)); // Light gray for placeholder
    placeholderTextDisplay.setString(placeholderText);
    placeholderTextDisplay.setPosition(inputText.getPosition());
}

void Input::draw(sf::RenderWindow& window) {
    window.draw(inputBox);
    if (text.empty() && !isFocused) {
        window.draw(placeholderTextDisplay); // Draw placeholder when input is empty and not focused
    } else {
        window.draw(inputText);
    }
}

void Input::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        isFocused = inBounds(mousePos);
    }

    if (isFocused && event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) {
            if (!text.empty()) {
                text.pop_back();
            }
        } else if (event.text.unicode < 128) {
            char enteredChar = static_cast<char>(event.text.unicode);
            if (onlyNumbers) {
                if (std::isdigit(enteredChar)) {
                    text += enteredChar;
                }
            } else {
                text += enteredChar;
            }
        }
        inputText.setString(text);
        if (onClickWithString) {
            onClickWithString(text);
        }
    }
}

// Getter for the text
const std::string& Input::getText() const {
    return text;
}

// Setter for the text
void Input::setText(const std::string& newText) {
    text = newText;
    inputText.setString(text);
}

// Setter and Getter for onlyNumbers
void Input::setOnlyNumbers(bool enabled) {
    onlyNumbers = enabled;
}

bool Input::getOnlyNumbers() const {
    return onlyNumbers;
}

// Setter and Getter for placeholder
void Input::setPlaceholder(const std::string& placeholder) {
    placeholderText = placeholder;
    placeholderTextDisplay.setString(placeholderText);
}

const std::string& Input::getPlaceholder() const {
    return placeholderText;
}

bool Input::inBounds(const sf::Vector2i& mousePos) const {
    return inputBox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}
