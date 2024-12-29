#ifndef SETTINGS_VIEW_HPP
#define SETTINGS_VIEW_HPP

#include <queue>

#include "View.hpp"

class SettingsView : public View {
public:
    SettingsView();
    void onActivate() override;
    void draw(sf::RenderWindow &window) override;
    void handleEvent(const sf::Event &event, sf::RenderWindow &window) override;
private:
    void createUI();
    std::queue<std::function<void()>> pendingOperations;
};

#endif
