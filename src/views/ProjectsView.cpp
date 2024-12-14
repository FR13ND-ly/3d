#include "ProjectsView.hpp"
#include "ViewsManager.hpp"

ProjectsView::ProjectsView() {
    createUI();
}

void ProjectsView::createUI() {
    auto myButton = std::make_shared<Button>(
        sf::Vector2f(100, 100),
        sf::Vector2f(200, 50),
        "Click Me",
        sf::Color::Red
    );

    myButton->setOnClick([]() {
        ViewsManager::getInstance().switchTo("editor");
    });

    auto myText = std::make_shared<Text>(
        sf::Vector2f(400, 100),
        20,
        "Projects Page"
    );

    auto myText2 = std::make_shared<Text>(
        sf::Vector2f(400, 200),
        20,
        "Salut Robert"
    );

    // Add components to the view
    this->addComponent(myText);
    this->addComponent(myText2);
    this->addComponent(std::dynamic_pointer_cast<Component>(myButton));
}

void ProjectsView::onActivate() {
    // Add activation logic here if needed
}

void ProjectsView::onDeactivate() {
    // Add deactivation logic here if needed
}
