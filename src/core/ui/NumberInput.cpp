#include "NumberInput.hpp"
#include <cctype>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>

NumberInput::NumberInput(const sf::Vector2f& position, const sf::Vector2f& size,const std::string& titleText,
                         float step, const sf::Color& boxColor, const sf::Color& textColor,
                         const std::string& placeholder, float minValue, float maxValue)
    : boxColor(boxColor), textColor(textColor), placeholderText(placeholder),
      step(step), minValue(minValue), maxValue(maxValue) {

    inputBox.setSize(size);
    inputBox.setPosition(position);
    inputBox.setFillColor(boxColor);
    inputBox.setOutlineColor(sf::Color::Black);
    inputBox.setOutlineThickness(1.f);

    inputText.setFont(FontManager::getInstance().getFont());
    inputText.setCharacterSize(static_cast<unsigned int>(size.y * 0.55f));
    inputText.setFillColor(textColor);
    inputText.setPosition(position.x + 5, position.y + (size.y * 0.25f));

    title.setFont(FontManager::getInstance().getFont());
    title.setCharacterSize(static_cast<unsigned int>(size.y * 0.25f));
    title.setFillColor(textColor);
    title.setString(titleText);
    title.setPosition(position.x + 5, position.y + 1.5f);

    placeholderTextDisplay.setFont(FontManager::getInstance().getFont());
    placeholderTextDisplay.setCharacterSize(static_cast<unsigned int>(size.y * 0.6f));
    placeholderTextDisplay.setFillColor(sf::Color(150, 150, 150));
    placeholderTextDisplay.setPosition(position.x + 5, position.y + (size.y * 0.2f));
    placeholderTextDisplay.setString(placeholder);
}

void NumberInput::draw(sf::RenderWindow& window) {
    window.draw(inputBox);
    if (isFocused) inputText.setColor(sf::Color::Black);
    else inputText.setColor(sf::Color(150, 150, 150));
    window.draw(title);
    if (displayValue.empty() && !isFocused) {
        window.draw(placeholderTextDisplay);
    } else {
        inputText.setString(displayValue);
        window.draw(inputText);
    }
}

void NumberInput::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        bool wasAlreadyFocused = isFocused;
        isFocused = inBounds(sf::Mouse::getPosition(window));

        if (wasAlreadyFocused && !isFocused) {
            validateAndUpdateValue();
        }
    }

    if (event.type == sf::Event::MouseWheelScrolled && inBounds(sf::Mouse::getPosition(window))) {
        if (!isFocused) {
            isFocused = true;
        }

        try {
            if (!displayValue.empty()) {
                numericValue = std::stof(displayValue);
                if (onClickWithFloat) {
                    onClickWithFloat(numericValue);
                }
            }
        } catch (...) {
            // If conversion fails, use current numeric value
        }

        // Increment or decrement value by step
        if (event.mouseWheelScroll.delta > 0) {
            numericValue += step;
        } else {
            numericValue -= step;
        }

        clampNumericValue();

        updateDisplayValue();
        if (onClickWithFloat) {
            onClickWithFloat(numericValue);
        }
    }
    if (isFocused) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::BackSpace) {
                if (!displayValue.empty()) {
                    displayValue.pop_back();
                    if (displayValue.empty()) {
                        numericValue = 0.0f;
                    } else {
                        try {
                            numericValue = std::stof(displayValue);
                        } catch (...) {
                            numericValue = 0.0f;
                        }
                        if (onClickWithFloat) {
                            onClickWithFloat(numericValue);
                        }
                    }
                }
            }
            else if (event.key.code == sf::Keyboard::Delete) {
                displayValue.clear();
                numericValue = 0.0f;
                if (onClickWithFloat) {
                    onClickWithFloat(numericValue);
                }
            }
        }
        else if (event.type == sf::Event::TextEntered) {
            char enteredChar = static_cast<char>(event.text.unicode);

            if (enteredChar < 32 || enteredChar > 126) return;

            if (enteredChar == '.' || enteredChar == '-' || std::isdigit(enteredChar)) {
                int dotCount = std::count(displayValue.begin(), displayValue.end(), '.');
                int minusCount = std::count(displayValue.begin(), displayValue.end(), '-');

                if ((enteredChar == '.' && dotCount == 0) ||
                    (enteredChar == '-' && minusCount == 0 && displayValue.empty()) ||
                    std::isdigit(enteredChar)) {
                    displayValue += enteredChar;
                }
            }
            if (onClickWithFloat) {
                onClickWithFloat(numericValue);
            }
        }
    }
}

void NumberInput::validateAndUpdateValue() {
    try {
        if (!displayValue.empty()) {
            numericValue = std::stof(displayValue);
            clampNumericValue();
            updateDisplayValue();
        } else {
            numericValue = 0.0f;
            displayValue = "0.00";
        }
    } catch (...) {
        updateDisplayValue();
    }
}

void NumberInput::updateDisplayValue() {
    std::ostringstream oss;
    oss.precision(2);
    oss << std::fixed << numericValue;
    displayValue = oss.str();
}

float NumberInput::getValue() const {
    return numericValue;
}

void NumberInput::setValue(float newValue) {
    numericValue = std::max(minValue, std::min(newValue, maxValue));
    updateDisplayValue();
}

void NumberInput::setPlaceholder(const std::string& placeholder) {
    placeholderText = placeholder;
    placeholderTextDisplay.setString(placeholderText);
}

const std::string& NumberInput::getPlaceholder() const {
    return placeholderText;
}

void NumberInput::clampNumericValue() {
    numericValue = std::max(minValue, std::min(numericValue, maxValue));
}

void NumberInput::setMinValue(float min) {
    minValue = min;
    clampNumericValue();
    updateDisplayValue();
}

void NumberInput::setMaxValue(float max) {
    maxValue = max;
    clampNumericValue();
    updateDisplayValue();
}

bool NumberInput::inBounds(const sf::Vector2i& mousePos) const {
    std::cout << inputBox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y)) << std::endl;
    return inputBox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}