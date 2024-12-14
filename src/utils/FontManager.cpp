#include "FontManager.hpp"
#include <iostream>
#include <cstdlib>

FontManager& FontManager::getInstance() {
    static FontManager instance;
    return instance;
}

const sf::Font& FontManager::getFont() const {
    return font;
}

FontManager::FontManager() {
    if (!font.loadFromFile("../src/assets/fonts/WorkSans.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
