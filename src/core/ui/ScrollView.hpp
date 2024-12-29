#ifndef SCROLL_VIEW_HPP
#define SCROLL_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Component.hpp"

class ScrollView : public Component {
public:
    ScrollView(const sf::Vector2f& position, const sf::Vector2f& size, float maxHeight);

    // Inherited from Component
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;
    void draw(sf::RenderWindow& window) override;

    // Add a component to the scroll view
    void addComponent(std::shared_ptr<Component> component);

    std::vector<std::shared_ptr<Component>> getComponents() const;
    // Clear all components
    void clearComponents();

    // Scroll-specific methods
    void scrollUp();
    void scrollDown();

    // Getters and Setters
    float getScrollOffset() const;
    void setScrollOffset(float offset);

    float getHeight() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

protected:
    // Override inBounds to check if point is within scroll view
    bool inBounds(const sf::Vector2i &mousePos) const override;

private:
    sf::Vector2f position;
    sf::Vector2f size;
    float maxHeight;

    std::vector<std::shared_ptr<Component>> components;

    float scrollOffset;
    sf::RectangleShape background;
    sf::RectangleShape scrollBar;
    sf::RectangleShape scrollHandle;
    bool isScrolling;

    // Calculation methods
    void updateScrollBar();
    void updateComponentPositions();
    float getTotalContentHeight() const;
    void handleScrolling(const sf::Event &event, const sf::RenderWindow &window);
    void setBackgroundColor(const sf::Color& color);
};

#endif // SCROLL_VIEW_HPP