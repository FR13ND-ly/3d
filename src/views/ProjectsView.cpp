#include "ProjectsView.hpp"
#include "ViewsManager.hpp"
#include "../utils/LanguageManager.hpp"

ProjectsView::ProjectsView() {}

void ProjectsView::createUI() {
    // Clear existing components to prevent overlap
    this->components.clear();

    // Fetch the updated language pack
    auto languagePack = LanguageManager::getInstance().getSelectedPack();

    // Verify keys exist to prevent errors
    std::string buttonLabel = languagePack.contains("Click Me") ? languagePack["Click Me"] : "Click Me";
    std::string pageLabel = languagePack.contains("Projects Page") ? languagePack["Projects Page"] : "Projects Page";

    // Add Button
    auto myButton = std::make_shared<Button>(
        sf::Vector2f(100, 100),
        sf::Vector2f(200, 50),
        buttonLabel,
        sf::Color::Red
    );

    myButton->setOnClick([]() {
        ViewsManager::getInstance().switchTo("editor");
    });

    // Add Text
    auto myText = std::make_shared<Text>(
        sf::Vector2f(400, 100),
        20,
        pageLabel
    );

    // Add components to the view
    this->addComponent(myText);
    this->addComponent(std::dynamic_pointer_cast<Component>(myButton));
}

void ProjectsView::onActivate() {
    // Completely rebuild UI when activated
    createUI();
}

void ProjectsView::onDeactivate() {
    // Ensure components are cleared when leaving the view
    this->components.clear();
}
