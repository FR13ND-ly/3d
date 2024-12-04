#ifndef VIEWS_MANAGER_HPP
#define VIEWS_MANAGER_HPP

#include <map>
#include <memory>
#include <string>
#include "View.hpp"
#include "HomeView.hpp"
#include "ProjectsView.hpp"
#include "EditorView.hpp"

class ViewsManager {
public:
    ViewsManager() {
        this->addView("home", std::make_shared<HomeView>());
        this->addView("projects", std::make_shared<ProjectsView>());
        this->addView("editor", std::make_shared<EditorView>());
        this->switchTo("home");
    }

    static ViewsManager& getInstance() {
        static ViewsManager instance;
        return instance;
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

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
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

    ViewsManager(const ViewsManager&) = delete;
    ViewsManager& operator=(const ViewsManager&) = delete;
};

#endif
