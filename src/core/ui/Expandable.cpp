#include "Expandable.hpp"

#include <iostream>

#include "../../utils/FontManager.hpp"
#include <limits>

Expandable::Expandable (
    const sf::Vector2f& position,
    const sf::Vector2f& size,
    const std::string& text,
    const sf::Color& color,
    const sf::Color& textColor
) : isCurrentlyExpanded(false), maxContentHeight(std::numeric_limits<float>::max()),
    headerColor(color) {
    headerRect.setSize(size);
    headerRect.setPosition(position);
    headerRect.setFillColor(color);

    const sf::Font &font = FontManager::getInstance().getFont();
    headerText.setFont(font);
    headerText.setString(text);
    headerText.setCharacterSize(static_cast<unsigned int>(size.y * 0.5f));
    headerText.setFillColor(textColor);

    // Center the text
    sf::FloatRect textBounds = headerText.getLocalBounds();
    headerText.setOrigin(textBounds.width / 2, textBounds.height / 2);
    headerText.setPosition(
        position.x + size.x / 2,
        position.y + size.y / 2 - textBounds.height / 2
    );
}

void Expandable::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    sf::Vector2i mousePos;
    if (event.type == sf::Event::MouseMoved) {
        mousePos = {event.mouseMove.x, event.mouseMove.y};
    } else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased) {
        mousePos = {event.mouseButton.x, event.mouseButton.y};
    } else {
        return;
    }

    bool isHovered = inBounds(mousePos);

    if (isHovered) {
        headerRect.setFillColor(darkenColor(headerColor, 20));
        if (onHover) {
            onHover();
        }
    } else {
        headerRect.setFillColor(headerColor);
        if (onHover) {
            onHover();
        }
    }

    if (isHovered && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        headerRect.setFillColor(darkenColor(headerColor, 60));
        // if (!isCurrentlyExpanded) {
            setExpanded(!isCurrentlyExpanded);
        // }
        if (onClick) {
            onClick();
        }
    }

    if (isCurrentlyExpanded) {
        for (auto& component : contentComponents) {
            component->handleEvent(event, window);
        }
    }
}

void Expandable::draw(sf::RenderWindow& window) {
    window.draw(headerRect);
    window.draw(headerText);

    if (isCurrentlyExpanded) {
        for (auto& component : contentComponents) {
            component->draw(window);
        }
    }
}

void Expandable::addContent(std::shared_ptr<Component> component) {
    contentComponents.push_back(std::move(component));
    updateContentPositions();
}

void Expandable::updateContentPositions() {
    if (contentComponents.empty()) return;

    float currentY = headerRect.getPosition().y + headerRect.getSize().y;
    for (auto& component : contentComponents) {
        sf::Vector2f position(
            component->getPosition().x,
            // headerRect.getPosition().x + (maxWidth - component->getHeight()) / 2,
            currentY
        );
        component->setPosition(position);
        currentY += component->getHeight();
    }
}

void Expandable::setExpanded(bool expanded) {
    isCurrentlyExpanded = expanded;
    updateContentPositions();
    if (onClickWithBool) {
        onClickWithBool(expanded);
    }
}

bool Expandable::isExpanded() const {
    return isCurrentlyExpanded;
}

void Expandable::setMaxContentHeight(float maxHeight) {
    maxContentHeight = maxHeight;
    updateContentPositions();
}

bool Expandable::inBounds(const sf::Vector2i& mousePos) const {
    return headerRect.getGlobalBounds().contains(
        static_cast<float>(mousePos.x),
        static_cast<float>(mousePos.y)
    );
}

float Expandable::getHeight() const {
    if (!isCurrentlyExpanded) {
        return headerRect.getSize().y;
    }

    // Calculate total height including header and content
    float totalHeight = headerRect.getSize().y;
    for (const auto& component : contentComponents) {
        totalHeight += component->getHeight();
    }
    return totalHeight;
}

sf::Vector2f Expandable::getPosition() const {
    return headerRect.getPosition();
}

void Expandable::setPosition(const sf::Vector2f& position) {
    // Store the original position difference
    sf::Vector2f originalPos = headerRect.getPosition();
    sf::Vector2f positionDelta = position - originalPos;

    // Move header
    headerRect.setPosition(position);

    // Adjust text position to remain centered
    sf::FloatRect textBounds = headerText.getLocalBounds();
    headerText.setPosition(
        position.x + headerRect.getSize().x / 2,
        position.y + headerRect.getSize().y / 2 - textBounds.height / 2
    );

    // Move content components relative to the header
    if (isCurrentlyExpanded) {
        for (auto& component : contentComponents) {
            sf::Vector2f currentCompPos = component->getPosition();
            component->setPosition(currentCompPos + positionDelta);
        }
    }
}

sf::Color Expandable::darkenColor(const sf::Color& color, int amount) {
    return sf::Color(
        std::max(0, color.r - amount),
        std::max(0, color.g - amount),
        std::max(0, color.b - amount)
    );
}

void Expandable::setColor(sf::Color color) {
    headerRect.setFillColor(color);
}

void Expandable::setTextColor(sf::Color color) {
    headerText.setFillColor(color);
}


std::vector<std::shared_ptr<Component>> Expandable::getContent() const {
    return contentComponents;
}
