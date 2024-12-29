// Container.hpp
#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Component.hpp"

class Container : public Component {
public:
    Container(const sf::Vector2f& position, const sf::Vector2f& size);

    // Inherited from Component
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;
    void draw(sf::RenderWindow& window) override;

    // Component management
    void addComponent(std::shared_ptr<Component> component);
    void clearComponents();
    std::vector<std::shared_ptr<Component>> getComponents() const;

    // Getters and Setters
    float getHeight() const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

protected:
    bool inBounds(const sf::Vector2i &mousePos) const override;

private:
    sf::Vector2f position;
    sf::Vector2f size;
    std::vector<std::shared_ptr<Component>> components;
    sf::RectangleShape background;

    void updateComponentPositions();
};

#endif // CONTAINER_HPP
