#ifndef PROJECTS_VIEW_HPP
#define PROJECTS_VIEW_HPP

#include <queue>

#include "View.hpp"

class ProjectsView: public View {
public:
    ProjectsView();

    void onActivate() override;
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
private:
    void createUI();
    std::queue<std::function<void()>> pendingOperations;
};

#endif
