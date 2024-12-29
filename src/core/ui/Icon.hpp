#ifndef ICON_HPP
#define ICON_HPP

#include "Component.hpp"
#include <string>

class Icon : public Component {
public:
    Icon(const sf::Vector2f& position, const sf::Vector2f& size, const std::string& iconName);

    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    float getHeight() const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

    void setIconName(const std::string& iconName);
    void setSize(const sf::Vector2f& size);

protected:
    bool inBounds(const sf::Vector2i& mousePos) const override;

private:
    sf::Sprite sprite;
    sf::Texture texture;
    std::string iconName;
};

#endif