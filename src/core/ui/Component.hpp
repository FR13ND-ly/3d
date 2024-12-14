#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/Graphics.hpp>
#include <functional>

class Component {
public:
    virtual ~Component() = default;

    virtual void handleEvent(const sf::Event &event, const sf::RenderWindow &window) = 0;

    virtual void draw(sf::RenderWindow& window) = 0;

    void setOnClick(const std::function<void()>& callback);
    void setOnClick(const std::function<void(bool)>& callback);
    void setOnClick(const std::function<void(float)>& callback);
    void setOnClick(const std::function<void(std::string)>& callback);

protected:
    std::function<void()> onClick;
    std::function<void(bool)> onClickWithBool;
    std::function<void(float)> onClickWithFloat;
    std::function<void(std::string)> onClickWithString;
    std::function<void(bool)> onHover;

    virtual bool inBounds(const sf::Vector2i &mousePos) const;
};

#endif
