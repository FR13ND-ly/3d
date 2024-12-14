#ifndef SCROLLVIEW_HPP
#define SCROLLVIEW_HPP

#include <SFML/Graphics.hpp>
#include "Component.hpp"
#include <vector>
#include <memory>

class ScrollView : public Component {
public:
    ScrollView(const sf::Vector2f& position, const sf::Vector2f& size, const sf::Color& backgroundColor = sf::Color::White, const sf::Color& scrollbarColor = sf::Color::Black)
        : size(size), backgroundColor(backgroundColor), scrollbarColor(scrollbarColor), offset(0.0f) {

        background.setSize(size);
        background.setPosition(position);
        background.setFillColor(backgroundColor);

        scrollbar.setSize({ 10.0f, size.y });
        scrollbar.setPosition(position.x + size.x - 10.0f, position.y);
        scrollbar.setFillColor(scrollbarColor);
    }

    void addComponent(const std::shared_ptr<Component>& component) {
        components.push_back(component);
    }

    void draw(sf::RenderWindow& window) override {
        sf::View originalView = window.getView();

        // Clip the ScrollView area
        sf::View scrollView;
        scrollView.setViewport({
            background.getPosition().x / window.getSize().x,
            background.getPosition().y / window.getSize().y,
            size.x / window.getSize().x,
            size.y / window.getSize().y
        });
        scrollView.setCenter(background.getPosition() + size / 2.0f);
        scrollView.setSize(size);

        window.setView(scrollView);

        // Draw components
        for (auto& component : components) {
            component->draw(window);
        }

        window.setView(originalView);

        // Draw background and scrollbar
        window.draw(background);
        if (needsScrollbar()) {
            window.draw(scrollbar);
        }
    }

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override {
        if (event.type == sf::Event::MouseWheelScrolled && needsScrollbar()) {
            offset -= event.mouseWheelScroll.delta * 10.0f;
            offset = std::max(0.0f, std::min(offset, getMaxScrollOffset()));
            updateComponentPositions();
        }

        for (auto& component : components) {
            component->handleEvent(event, window);
        }
    }

private:
    sf::RectangleShape background;
    sf::RectangleShape scrollbar;
    std::vector<std::shared_ptr<Component>> components;
    sf::Vector2f size;
    sf::Color backgroundColor;
    sf::Color scrollbarColor;
    float offset;

    bool needsScrollbar() const {
        float totalHeight = 0.0f;
        for (const auto& component : components) {
            totalHeight += component->getGlobalBounds().height;
        }
        return totalHeight > size.y;
    }

    float getMaxScrollOffset() const {
        float totalHeight = 0.0f;
        for (const auto& component : components) {
            totalHeight += component->getGlobalBounds().height;
        }
        return std::max(0.0f, totalHeight - size.y);
    }

    void updateComponentPositions() {
        float yOffset = -offset;
        for (auto& component : components) {
            sf::Vector2f position = component->getPosition();
            position.y = yOffset;
            component->setPosition(position);
            yOffset += component->getGlobalBounds().height;
        }

        float scrollbarHeightRatio = size.y / (size.y + getMaxScrollOffset());
        scrollbar.setSize({ scrollbar.getSize().x, size.y * scrollbarHeightRatio });
        scrollbar.setPosition(scrollbar.getPosition().x, background.getPosition().y + (offset / getMaxScrollOffset()) * (size.y - scrollbar.getSize().y));
    }
};

#endif