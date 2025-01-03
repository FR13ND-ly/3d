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
    void setOnHover(const std::function<void()>& callback);
    void setOnHoverOut(const std::function<void()>& callback);
    virtual float getHeight() const = 0;

    virtual sf::Vector2f getPosition() const = 0;
    virtual void setPosition(const sf::Vector2f& position) = 0;;
protected:
    std::function<void()> onClick;
    std::function<void(bool)> onClickWithBool;
    std::function<void(float)> onClickWithFloat;
    std::function<void(std::string)> onClickWithString;
    std::function<void()> onHover;
    std::function<void()> onHoverOut;

    std::function<void(float)> onInputWthFloat;
    std::function<void(std::string)> onInputWithString;
    std::function<void(float)> onScroll;

    virtual bool inBounds(const sf::Vector2i &mousePos) const;
};

#endif
