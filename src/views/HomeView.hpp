#ifndef HOME_VIEW_HPP
#define HOME_VIEW_HPP

#include "View.hpp"

class HomeView : public View {
public:
    HomeView();

    void onActivate() override;
    void draw(sf::RenderWindow &window) override;

private:
    void createUI();
    sf::Texture bgTexture;
    sf::Sprite bgSprite;
};

#endif
