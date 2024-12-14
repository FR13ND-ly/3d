#include "ViewsManager.hpp"

ViewsManager::ViewsManager() {
    this->addView("home", std::make_shared<HomeView>());
    this->addView("projects", std::make_shared<ProjectsView>());
    this->addView("editor", std::make_shared<EditorView>());
    this->addView("settings", std::make_shared<SettingsView>());
    this->switchTo("home");
}

ViewsManager& ViewsManager::getInstance() {
    static ViewsManager instance;
    return instance;
}

void ViewsManager::addView(const std::string& name, std::shared_ptr<View> view) {
    views[name] = view;
}

void ViewsManager::switchTo(const std::string& name) {
    if (currentView) {
        currentView->onDeactivate();
    }

    if (views.find(name) != views.end()) {
        currentView = views[name];
        currentView->onActivate();
    }
}

void ViewsManager::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (currentView) {
        currentView->handleEvent(event, window);
    }
}

void ViewsManager::draw(sf::RenderWindow& window) {
    if (currentView) {
        currentView->draw(window);
    }
}
