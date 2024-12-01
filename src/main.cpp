#include <SFML/Graphics.hpp>

int main() {
    // Create a window with the specified width, height, and title
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Red Circle");

    // Create a red circle with a radius of 50
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Red);

    // Set the position of the circle to the center of the window
    circle.setPosition(375, 275);  // (800 / 2 - 50, 600 / 2 - 50)

    // Main game loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close(); // Close the window when the user clicks the close button
            }
        }

        // Clear the window with a color (black in this case)
        window.clear(sf::Color::Black);

        // Draw the red circle to the window
        window.draw(circle);

        // Display the window content
        window.display();
    }

    return 0;
}
