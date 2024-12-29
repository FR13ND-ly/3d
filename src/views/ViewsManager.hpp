#ifndef VIEWS_MANAGER_HPP
#define VIEWS_MANAGER_HPP

#include <map>
#include <memory>
#include <string>
#include "View.hpp"
#include "HomeView.hpp"
#include "ProjectsView.hpp"
#include "EditorView.hpp"
#include "SettingsView.hpp"

class ViewsManager {
public:
    ViewsManager();

    static ViewsManager& getInstance();

    void addView(const std::string& name, std::shared_ptr<View> view);
    void switchTo(const std::string& name);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    std::map<std::string, std::shared_ptr<View>> views;
    std::shared_ptr<View> currentView = nullptr;
    std::queue<std::function<void()>> pendingOperations;

    ViewsManager(const ViewsManager&) = delete;
    ViewsManager& operator=(const ViewsManager&) = delete;
};

#endif
