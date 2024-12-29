#include "Snackbar.hpp"

Snackbar::Snackbar(const sf::Vector2f& position, const sf::Vector2f& size) {
    // Setup shadow
    shadowBox.setSize(size);
    shadowBox.setFillColor(sf::Color(0, 0, 0, 100));  // Semi-transparent black
    shadowBox.setPosition(position.x + SHADOW_OFFSET, position.y + SHADOW_OFFSET);

    // Setup main message box
    messageBox.setSize(size);
    messageBox.setPosition(position);

    const sf::Font& font = FontManager::getInstance().getFont();
    messageText.setFont(font);
    messageText.setCharacterSize(static_cast<unsigned int>(size.y * 0.4f));
    messageText.setFillColor(sf::Color::White);
}

void Snackbar::draw(sf::RenderWindow& window) {
    if (!isDisplaying || messageQueue.empty()) {
        return;
    }

    window.draw(shadowBox);  // Draw shadow first
    window.draw(messageBox);
    window.draw(messageText);
}

void Snackbar::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    // Could add click-to-dismiss functionality here if desired
}

void Snackbar::addMessage(const std::string& message, const sf::Color& bgColor) {
    SnackbarMessage newMessage{message, bgColor, sf::Clock()};
    messageQueue.push(newMessage);

    if (!isDisplaying) {
        showNextMessage();
    }
}

void Snackbar::update() {
    if (!isDisplaying || messageQueue.empty()) {
        return;
    }

    if (messageQueue.front().timer.getElapsedTime().asSeconds() >= DISPLAY_DURATION) {
        messageQueue.pop();
        if (!messageQueue.empty()) {
            showNextMessage();
        } else {
            isDisplaying = false;
        }
    }
}

void Snackbar::showNextMessage() {
    if (messageQueue.empty()) {
        isDisplaying = false;
        return;
    }

    isDisplaying = true;
    SnackbarMessage& currentMessage = messageQueue.front();

    messageBox.setFillColor(currentMessage.backgroundColor);
    messageText.setString(currentMessage.text);

    // Center the text in the message box
    sf::FloatRect textBounds = messageText.getLocalBounds();
    messageText.setOrigin(textBounds.width / 2, textBounds.height / 2 + textBounds.top);
    messageText.setPosition(
        messageBox.getPosition().x + messageBox.getSize().x / 2,
        messageBox.getPosition().y + messageBox.getSize().y / 2
    );

    currentMessage.timer.restart();
}

void Snackbar::updateShadowPosition() {
    sf::Vector2f mainPos = messageBox.getPosition();
    shadowBox.setPosition(mainPos.x + SHADOW_OFFSET, mainPos.y + SHADOW_OFFSET);
}

float Snackbar::getHeight() const {
    return messageBox.getSize().y;
}

sf::Vector2f Snackbar::getPosition() const {
    return messageBox.getPosition();
}

void Snackbar::setPosition(const sf::Vector2f& position) {
    messageBox.setPosition(position);
    updateShadowPosition();

    if (!messageQueue.empty()) {
        // Update text position
        sf::FloatRect textBounds = messageText.getLocalBounds();
        messageText.setOrigin(textBounds.width / 2, textBounds.height / 2 + textBounds.top);
        messageText.setPosition(
            position.x + messageBox.getSize().x / 2,
            position.y + messageBox.getSize().y / 2
        );
    }
}

bool Snackbar::inBounds(const sf::Vector2i& mousePos) const {
    return messageBox.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}