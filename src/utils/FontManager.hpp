#ifndef FONT_MANAGER_HPP
#define FONT_MANAGER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class FontManager {
public:
    static FontManager& getInstance() {
        static FontManager instance;
        return instance;
    }

    const sf::Font& getFont() const {
        return font;
    }

private:
    sf::Font font;

    FontManager() {
        if (!font.loadFromFile("../src/assets/fonts/WorkSans.ttf")) {
            std::cerr << "Failed to load font!" << std::endl;
            // std::exit(EXIT_FAILURE);
        }
    }

    FontManager(const FontManager&) = delete;
    FontManager& operator=(const FontManager&) = delete;
};

#endif
