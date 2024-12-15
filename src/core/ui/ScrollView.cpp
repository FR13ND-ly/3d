#include "ScrollView.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>

ScrollView::ScrollView(const sf::Vector2f& position, const sf::Vector2f& size, float maxHeight)
    : position(position), size(size), maxHeight(maxHeight), scrollOffset(0), isScrolling(false) {

    background.setSize(size);
    background.setPosition(position);
    background.setFillColor(sf::Color(40, 40, 40));

    scrollBar.setSize(sf::Vector2f(10.f, maxHeight));
    scrollBar.setPosition(position.x + size.x - 10.f, position.y);
    scrollBar.setFillColor(sf::Color(200, 200, 200));

    scrollHandle.setSize(sf::Vector2f(10.f, 50.f));
    scrollHandle.setFillColor(sf::Color(150, 150, 150));
}

void ScrollView::updateComponentPositions() {
    float currentY = position.y;

    for (auto& component : components) {
        component->setPosition({position.x, currentY});
        currentY += component->getHeight();
    }

    updateScrollBar(); // Ensure the scrollbar reflects the updated total height
}

void ScrollView::addComponent(std::shared_ptr<Component> component) {
    // Set the position of the new component below the existing ones
    float currentY = position.y;

    for (const auto& existingComponent : components) {
        currentY += existingComponent->getHeight();
    }

    component->setPosition({position.x, currentY});
    components.push_back(component);

    // Update the scrollbar to reflect the new total content height
    updateScrollBar();
}

void ScrollView::clearComponents() {
    components.clear();
    scrollOffset = 0;
    updateScrollBar();
}

void ScrollView::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (!inBounds(sf::Mouse::getPosition(window))) return;
    float totalHeight = getTotalContentHeight();
    if (totalHeight <= maxHeight) {
        for (auto& component : components) {
            component->handleEvent(event, window);
        }
        return;
    }

    handleScrolling(event, window);

    sf::Event adjustedEvent = event;
    if (event.type == sf::Event::MouseButtonPressed ||
        event.type == sf::Event::MouseButtonReleased ||
        event.type == sf::Event::MouseMoved) {

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2i adjustedMousePos(mousePos.x, mousePos.y + static_cast<int>(scrollOffset));

        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
            adjustedEvent.mouseButton.x = adjustedMousePos.x;
            adjustedEvent.mouseButton.y = adjustedMousePos.y;
        } else if (event.type == sf::Event::MouseMoved) {
            adjustedEvent.mouseMove.x = adjustedMousePos.x;
            adjustedEvent.mouseMove.y = adjustedMousePos.y;
        }

    }
    float currentY = position.y - scrollOffset;
    for (auto& component : components) {
        float componentHeight = component->getHeight();
        if (currentY + componentHeight >= position.y && currentY <= position.y + maxHeight) {
            component->handleEvent(adjustedEvent, window);
        }
        currentY += componentHeight;
    }
}

void ScrollView::draw(sf::RenderWindow& window) {
    window.draw(background);
    updateComponentPositions();
    float totalHeight = getTotalContentHeight();

    // If content fits within the visible area, draw all components
    if (totalHeight <= maxHeight) {
        for (const auto& component : components) {
            component->draw(window);
        }
        return;
    }

    // Apply a clipping view for the scroll area
    sf::View originalView = window.getView();

    sf::View clippingView = originalView;
    clippingView.setViewport(sf::FloatRect(
        position.x / window.getSize().x,
        position.y / window.getSize().y,
        size.x / window.getSize().x,
        maxHeight / window.getSize().y
    ));

    clippingView.setCenter(
        position.x + size.x / 2,
        position.y + maxHeight / 2 + scrollOffset
    );

    clippingView.setSize(size);
    window.setView(clippingView);

    // Draw visible components only
    float currentY = position.y - scrollOffset;
    for (const auto& component : components) {
        sf::Vector2f componentPosition = component->getPosition();
        float componentHeight = component->getHeight();

        if (currentY + componentHeight >= position.y && currentY <= position.y + maxHeight) {
            component->draw(window);
        }
        currentY += componentHeight;
    }

    // Reset to the original view
    window.setView(originalView);

    // Draw the scrollbar if needed
    if (totalHeight > maxHeight) {
        window.draw(scrollBar);
        window.draw(scrollHandle);
    }
}



void ScrollView::setBackgroundColor(const sf::Color& color) {
    background.setFillColor(color);
}

void ScrollView::handleScrolling(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    float totalHeight = getTotalContentHeight();

    if (event.type == sf::Event::MouseWheelScrolled) {
        if (inBounds(mousePos)) {
            scrollOffset -= event.mouseWheelScroll.delta * 20.f;
            scrollOffset = std::clamp(scrollOffset, 0.f, totalHeight - maxHeight);
            updateScrollBar();
        }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
        if (scrollHandle.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            isScrolling = true;
        }
    }

    if (event.type == sf::Event::MouseButtonReleased) {
        isScrolling = false;
    }

    if (isScrolling && event.type == sf::Event::MouseMoved) {
        float relativeMouseY = mousePos.y - scrollBar.getPosition().y;
        float handleHeight = scrollHandle.getSize().y;
        float scrollBarHeight = scrollBar.getSize().y;

        scrollOffset = (relativeMouseY / (scrollBarHeight - handleHeight)) * (totalHeight - maxHeight);
        scrollOffset = std::clamp(scrollOffset, 0.f, totalHeight - maxHeight - 10.f);
        updateScrollBar();
    }
}

void ScrollView::updateScrollBar() {
    float totalHeight = getTotalContentHeight();

    if (totalHeight <= maxHeight) {
        scrollHandle.setSize(sf::Vector2f(0, 0)); // Hide scrollbar
        return;
    }

    float scrollRatio = maxHeight / totalHeight;
    float handleHeight = std::max(30.f, scrollRatio * scrollBar.getSize().y); // Minimum height for usability
    scrollHandle.setSize(sf::Vector2f(scrollBar.getSize().x, handleHeight));

    // Calculate the scroll handle position
    float handlePositionRatio = scrollOffset / (totalHeight - maxHeight);
    float handleY = scrollBar.getPosition().y +
                    handlePositionRatio * (scrollBar.getSize().y - handleHeight);
    scrollHandle.setPosition(scrollBar.getPosition().x, handleY);
}

float ScrollView::getTotalContentHeight() const {
    float totalHeight = 0.f;
    for (const auto& component : components) {
        totalHeight += component->getHeight();
    }
    return totalHeight;
}

bool ScrollView::inBounds(const sf::Vector2i &mousePos) const {
    sf::FloatRect bounds(position.x, position.y, size.x, maxHeight);
    return bounds.contains(mousePos.x, mousePos.y);
}

float ScrollView::getScrollOffset() const {
    return scrollOffset;
}

void ScrollView::setScrollOffset(float offset) {
    float totalHeight = getTotalContentHeight();
    scrollOffset = std::max(0.f, std::min(offset, totalHeight - maxHeight));
    updateScrollBar();
}

void ScrollView::scrollUp() {
    scrollOffset = std::max(0.f, scrollOffset - 20.f);
    updateScrollBar();
}

void ScrollView::scrollDown() {
    float totalHeight = getTotalContentHeight();
    scrollOffset = std::min(scrollOffset + 20.f, totalHeight - maxHeight);
    updateScrollBar();
}

float ScrollView::getHeight() const {
    return background.getGlobalBounds().height;
}

sf::Vector2f ScrollView::getPosition() const {
    return position;
}

void ScrollView::setPosition(const sf::Vector2f &newPosition) {
    position = newPosition;

    // Update the position of the background
    background.setPosition(newPosition);

    // Update the position of the scroll bar and its handle
    scrollBar.setPosition(newPosition.x + size.x - 10.f, newPosition.y);
    scrollHandle.setPosition(scrollBar.getPosition().x, newPosition.y);

    // Optionally, update the positions of the components inside the scroll view
    float currentY = newPosition.y;
    for (auto& component : components) {
        sf::Vector2f originalPos = component->getPosition();
        component->setPosition({originalPos.x, currentY});
        currentY += component->getHeight();
    }

    updateScrollBar();  // Ensure the scroll bar is updated after repositioning
}