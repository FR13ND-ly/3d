#ifndef EXPANDABLE_COMPONENT_HPP
#define EXPANDABLE_COMPONENT_HPP

#include "Component.hpp"
#include <vector>
#include <memory>

class Expandable : public Component {
public:
    Expandable(
        const sf::Vector2f& position,
        const sf::Vector2f& size,
        const std::string& headerText,
        const sf::Color& headerColor = sf::Color(140, 140, 140),
        const sf::Color& headerTextColor = sf::Color::White
    );

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;
    void draw(sf::RenderWindow& window) override;
    float getHeight() const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

    void addContent(std::shared_ptr<Component> component);
    void setExpanded(bool expanded);
    bool isExpanded() const;

    void setColor(sf::Color color);
    void setTextColor(sf::Color color);

    std::vector<std::shared_ptr<Component>> getContent() const;

    void setMaxContentHeight(float maxHeight);

protected:
    bool inBounds(const sf::Vector2i& mousePos) const override;

private:
    sf::RectangleShape headerRect;
    sf::Text headerText;
    std::vector<std::shared_ptr<Component>> contentComponents;
    bool isCurrentlyExpanded;
    float maxContentHeight;
    sf::Color headerColor;

    void updateContentPositions();
    sf::Color darkenColor(const sf::Color& color, int amount);
};

#endif