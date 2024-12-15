#include "Slider.hpp"

Slider::Slider(const sf::Vector2f& position, const sf::Vector2f& size, float minValue, float maxValue, float currentValue, const sf::Color& trackColor, const sf::Color& handleColor)
    : minValue(minValue), maxValue(maxValue), currentValue(currentValue), isDragging(false), size(size), position(position) {

    // Initialize track
    track.setSize(size);
    track.setPosition(position);
    track.setFillColor(trackColor);

    handle.setSize({ size.y, size.y });
    handle.setOrigin(handle.getSize().x / 2, handle.getSize().y / 2);
    handle.setFillColor(handleColor);

    setValue(currentValue);
}

void Slider::draw(sf::RenderWindow& window) {
    window.draw(track);
    window.draw(handle);
}

void Slider::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (handle.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            isDragging = true;
        }
    } else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        isDragging = false;
    }

    if (isDragging && event.type == sf::Event::MouseMoved) {
        float newPos = static_cast<float>(mousePos.x);
        float leftBound = track.getPosition().x;
        float rightBound = track.getPosition().x + track.getSize().x;

        // Clamp the position of the handle within the bounds of the track
        if (newPos < leftBound) newPos = leftBound;
        if (newPos > rightBound) newPos = rightBound;

        handle.setPosition(newPos, handle.getPosition().y);

        // Update the value based on the handle's position
        currentValue = minValue + ((newPos - leftBound) / track.getSize().x) * (maxValue - minValue);
    }

    // Notify the callback function
    if (onClickWithFloat) {
        onClickWithFloat(currentValue);
    }
}

// Getter function definition
float Slider::getValue() const {
    return currentValue;
}

// Setter function definition
void Slider::setValue(float value) {
    currentValue = std::max(minValue, std::min(maxValue, value));
    float newPos = track.getPosition().x + ((currentValue - minValue) / (maxValue - minValue)) * track.getSize().x;
    handle.setPosition(newPos, position.y + (size.y / 2));
}

float Slider::getHeight() const {
    return size.y;
}

sf::Vector2f Slider::getPosition() const {
    return position;
}

void Slider::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;

    // Set the position of the track (the background of the slider)
    track.setPosition(position);

    // Set the position of the handle (the draggable part)
    handle.setPosition(position.x + ((currentValue - minValue) / (maxValue - minValue)) * track.getSize().x, position.y + (size.y / 2));
}