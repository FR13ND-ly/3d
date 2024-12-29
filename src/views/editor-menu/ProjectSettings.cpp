#include "ProjectSettings.hpp"

#include "../ViewsManager.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../core/feature/ProjectsManager.hpp"
#include "../../core/ui/Snackbar.hpp"
#include "../../utils/WindowManager.hpp"
#include "../../utils/LanguageManager.hpp"

ProjectSettings::ProjectSettings() {
}

void ProjectSettings::createUI() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();

    components.clear();

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 20),
        30,
        languagePack["project_settings"]
    );

    auto saveButton = std::make_shared<Button>(
        sf::Vector2f(parentPosition.x + 20, 90),
        sf::Vector2f(310, 50),
        languagePack["save"]
    );

    saveButton->setIcon("save");

    saveButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        ProjectsManager& projectsManager = ProjectsManager::getInstance();
        projectsManager.updateProject(scene);
    });

    auto exportButton = std::make_shared<Button>(
         sf::Vector2f(parentPosition.x + 20, 160),
         sf::Vector2f(310, 50),
         languagePack["export"]
    );

    exportButton->setIcon("export");

    exportButton->setOnClick([]() {;
        ProjectsManager& projectsManager = ProjectsManager::getInstance();
        if (projectsManager.exportAsObj()) {
            auto languagePack = LanguageManager::getInstance().getSelectedPack();
            Snackbar::getInstance().addMessage(languagePack["project_exported"]);
        }

    });

    auto deleteButton = std::make_shared<Button>(
         sf::Vector2f(parentPosition.x + 20, 270),
         sf::Vector2f(310, 50),
         languagePack["delete"]
    );

    deleteButton->setIcon("delete");

    deleteButton->setOnClick([]() {;
        ProjectsManager& projectsManager = ProjectsManager::getInstance();
        projectsManager.deleteCurrentProject();
        ViewsManager& viewsManager = ViewsManager::getInstance();
        viewsManager.switchTo("projects");
    });



    this->addComponent(title);
    this->addComponent(saveButton);
    this->addComponent(exportButton);
    this->addComponent(deleteButton);
}

void ProjectSettings::onActivate() {
    createUI();
}

void ProjectSettings::onDeactivate() {
    components.clear();
}


