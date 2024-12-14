#include "Select.hpp"
#include <iostream>


Select::Select(const sf::Vector2f& position, const sf::Vector2f& size, const std::vector<std::string>& options, int defaultIndex)
    : position(position), size(size), options(options), selectedIndex(defaultIndex), isOpen(false) {

    box.setSize({size.x, (options.size() + 1) * 30.f + 10});
    box.setPosition(position);
    box.setFillColor(sf::Color(50, 50, 50));

    font = FontManager::getInstance().getFont();

    selectedText.setFont(font);
    selectedText.setCharacterSize(20);
    selectedText.setFillColor(sf::Color::White);
    selectedText.setString(options[selectedIndex]);
    selectedText.setPosition(position.x + 10, position.y + 5);

    for (size_t i = 0; i < options.size(); ++i) {
        Button text = Button(
            {position.x, position.y + (i + 1) * 30 + 10},
            {size.x, 30},
            options[i],
            sf::Color(50, 50, 50),
            sf::Color::White
        );
        text.setOnClick([this, i]()-> void {
            if (isOpen) {
                selectedIndex = i;
                selectedText.setString(this->options[selectedIndex]);
                isOpen = false;
                if (onClickWithFloat) {
                    onClickWithFloat(static_cast<float>(selectedIndex));
                }
            }
        });
        optionTexts.push_back(text);
    }
}

void Select::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        if (box.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            isOpen = true;
        } else if (isOpen) {
            isOpen = false;
        }
        for (size_t i = 0; i < options.size(); ++i) {
            optionTexts[i].handleEvent(event, window);
        }

    }
}

void Select::draw(sf::RenderWindow& window) {
    if (isOpen) {
        window.draw(box);
        window.draw(selectedText);
        for (size_t i = 0; i < options.size(); ++i) {
            optionTexts[i].draw(window);
        }
    }
    else {
        window.draw(selectedText);
    }
}

std::string Select::getSelectedOption() const {
    return options[selectedIndex];
}

int Select::getSelectedIndex() const {
    return selectedIndex;
}

void Select::setOnSelect(const std::function<void(float)>& callback) {
    onClickWithFloat = callback;
}
