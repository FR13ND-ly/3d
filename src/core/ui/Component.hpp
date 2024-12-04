#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <functional>

class Component {
public:
    virtual ~Component() = default;

    virtual void handleEvent(const sf::Event& event, const sf::RenderWindow &window) {}

    virtual void draw(sf::RenderWindow& window) = 0;

    void setOnClick(const std::function<void()> &callback) {
        onClick = callback;
    }

  protected:
    std::function<void()> onClick;
    std::function<void(bool)> onHover;
    virtual bool inBounds(const sf::Vector2i &mousePos) const {};
};

#endif
