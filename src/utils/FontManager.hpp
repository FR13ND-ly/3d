#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP

#include <SFML/Graphics.hpp>

class FontManager {
public:
    static FontManager& getInstance();

    const sf::Font& getFont() const;

private:
    sf::Font font;

    FontManager();

    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
};

#endif
