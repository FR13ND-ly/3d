#include "Component.hpp"

// Setters for click event callbacks with different parameter types
void Component::setOnClick(const std::function<void()>& callback) {
    onClick = callback;
}

void Component::setOnClick(const std::function<void(bool)>& callback) {
    onClickWithBool = callback;
}

void Component::setOnClick(const std::function<void(float)>& callback) {
    onClickWithFloat = callback;
}

void Component::setOnClick(const std::function<void(std::string)>& callback) {
    onClickWithString = callback;
}

void Component::setOnHover(const std::function<void()> &callback) {
    onHover = callback;
}

void Component::setOnHoverOut(const std::function<void()> &callback) {
    onHoverOut = callback;
}




bool Component::inBounds(const sf::Vector2i &mousePos) const {
    return false;
}
