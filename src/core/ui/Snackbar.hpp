#ifndef SNACKBAR_HPP
#define SNACKBAR_HPP

#include <SFML/Graphics.hpp>
#include <queue>
#include <string>
#include "Component.hpp"
#include "../../utils/FontManager.hpp"

class Snackbar : public Component {
public:
    static Snackbar& getInstance() {
        static Snackbar instance({50, 950}, {300, 50}); // Default position near bottom of screen
        return instance;
    }

    // Delete copy constructor and assignment operator
    Snackbar(const Snackbar&) = delete;
    Snackbar& operator=(const Snackbar&) = delete;

    void draw(sf::RenderWindow& window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

    void addMessage(const std::string& message, const sf::Color& bgColor = sf::Color(50, 50, 50, 230));
    void update();

    float getHeight() const override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

private:
    Snackbar(const sf::Vector2f& position, const sf::Vector2f& size);

    struct SnackbarMessage {
        std::string text;
        sf::Color backgroundColor;
        sf::Clock timer;
    };

    std::queue<SnackbarMessage> messageQueue;
    sf::RectangleShape messageBox;
    sf::RectangleShape shadowBox;
    sf::Text messageText;

    const float DISPLAY_DURATION = 2.5f;
    const float SHADOW_OFFSET = 4.0f;
    bool isDisplaying = false;

    void showNextMessage();
    void updateShadowPosition();  // New helper method
    bool inBounds(const sf::Vector2i& mousePos) const override;
};

#endif