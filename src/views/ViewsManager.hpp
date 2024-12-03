#ifndef VIEW_MANAGER_HPP
#define VIEW_MANAGER_HPP

#include "View.hpp"
#include <map>
#include <memory>
#include <string>
#include "HomeView.hpp"

class ViewManager {
public:
    ViewManager() {
        this->addView("home", std::shared_ptr<View>(new HomeView()));
        this->switchTo("home");
    }

    void addView(const std::string& name, std::shared_ptr<View> view) {
        views[name] = view;
    }

    void switchTo(const std::string& name) {
        if (currentView) {
            currentView->onDeactivate();
        }

        if (views.find(name) != views.end()) {
            currentView = views[name];
            currentView->onActivate();
        }
    }

    void handleEvent(const sf::Event& event, const sf::RenderWindow &window) {
        if (currentView) {
            currentView->handleEvent(event, window);
        }
    }

    void draw(sf::RenderWindow& window) {
        if (currentView) {
            currentView->draw(window);
        }
    }

private:
    std::map<std::string, std::shared_ptr<View>> views;
    std::shared_ptr<View> currentView = nullptr;
};

#endif
