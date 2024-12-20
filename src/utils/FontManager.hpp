#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP

#include <SFML/Graphics.hpp>

class FontManager {
public:
    static FontManager& getInstance();

    const sf::Font& getFont() const;

private:
    // The font object
    sf::Font font;

    // Private constructor for singleton pattern
    FontManager();

    // Delete copy constructor and assignment operator
    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
};

#endif // FONT_MANAGER_HPP
