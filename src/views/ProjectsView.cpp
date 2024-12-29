#include "ProjectsView.hpp"
#include "ViewsManager.hpp"
#include "../utils/LanguageManager.hpp"
#include "../core/feature/ProjectsManager.hpp"
#include "../core/ui/ScrollView.hpp"
#include "../core/ui/Container.hpp"
#include "../core/ui/Surface.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../core/ui/Component.hpp"
#include "../core/ui/Text.hpp"
#include "../core/ui/Button.hpp"
#include "../core/ui/Snackbar.hpp"

ProjectsView::ProjectsView() {}

void ProjectsView::createUI() {
    this->components.clear();

    auto languagePack = LanguageManager::getInstance().getSelectedPack();

    auto headerSurface = std::make_shared<Surface>(
        sf::Vector2f(0, 0),
        sf::Vector2f(1920, 80),
        sf::Color(20, 20, 20)
    );

    auto backButton = std::make_shared<Button>(
        sf::Vector2f(20, 20),
        sf::Vector2f(100, 40),
        languagePack["back"]
    );

    backButton->setIcon("back");

    backButton->setOnClick([this]() {
        ViewsManager::getInstance().switchTo("home");
    });

    auto title = std::make_shared<Text>(
        sf::Vector2f(backButton->getWidth() + 40, 20),
        30.f,
        languagePack["projects"],
        sf::Color::White
    );

    auto addProjectButton = std::make_shared<Button>(
        sf::Vector2f(1720, 20),
        sf::Vector2f(180, 40),
        languagePack["new_project"]
    );

    addProjectButton->setIcon("add");
    addProjectButton->setPosition({1900 - addProjectButton->getWidth(), addProjectButton->getPosition().y});
    addProjectButton->setOnClick([]() {
        ProjectsManager& projectsManager = ProjectsManager::getInstance();
        projectsManager.createProject();
        ViewsManager::getInstance().switchTo("editor");
        Snackbar::getInstance().addMessage("Project created!");
    });

    auto scrollView = std::make_shared<ScrollView>(
        sf::Vector2f(20, 100),
        sf::Vector2f(1880, 930),
        930.f
    );

    int i = 0;
    ProjectsManager& projectsManager = ProjectsManager::getInstance();
    for (auto project: projectsManager.getProjects()) {
        auto container = std::make_shared<Container>(
            sf::Vector2f(20, 100 * i),
            sf::Vector2f(1880, 100)
        );
        auto projectTitle = std::make_shared<Text>(
            sf::Vector2f(40, 120 + 100 * i),
            20.f,
            project.fileName
        );
        auto projectDate = std::make_shared<Text>(
            sf::Vector2f(40, 150 + 100 * i),
            15.f,
            static_cast<std::string>(languagePack["date"]) + project.lastUpdateDate
        );
        auto deleteButton = std::make_shared<Button>(
            sf::Vector2f(500, 120 + 100 * i),
            sf::Vector2f(100, 50),
            languagePack["delete"]
        );
        deleteButton->setIcon("delete");
        deleteButton->setPosition({1880 - deleteButton->getWidth(), deleteButton->getPosition().y});

        deleteButton->setOnClick([this, project]() {
            ProjectsManager& projectsManager = ProjectsManager::getInstance();
            projectsManager.deleteProject(project.filePath);
            pendingOperations.push([this]() {
                createUI();
                auto languagePack = LanguageManager::getInstance().getSelectedPack();
                Snackbar::getInstance().addMessage(languagePack["project_deleted"]);
            });
        });

        auto openButton = std::make_shared<Button>(
            sf::Vector2f(500, 120 + 100 * i),
            sf::Vector2f(100, 50),
            languagePack["open"]
        );
        openButton->setIcon("open");
        openButton->setPosition({1860 - openButton->getWidth() - deleteButton->getWidth(), openButton->getPosition().y});

        openButton->setOnClick([project]() {
            ProjectsManager& projectsManager = ProjectsManager::getInstance();
            projectsManager.setProject(project.filePath);
            ViewsManager::getInstance().switchTo("editor");
        });

        auto divider = std::make_shared<Surface>(
            sf::Vector2f(20,  195 + 100 * i),
            sf::Vector2f(1880, 2),
            sf::Color(100, 100, 100)
        );

        container->addComponent(openButton);
        container->addComponent(deleteButton);
        container->addComponent(projectTitle);
        container->addComponent(projectDate);
        if (i + 1 < projectsManager.getProjects().size()) {
            container->addComponent(divider);
        }
        scrollView->addComponent(container);
        i++;
    }
    this->addComponent(headerSurface);
    this->addComponent(backButton);
    this->addComponent(title);
    this->addComponent(addProjectButton);
    this->addComponent(scrollView);
}

void ProjectsView::onActivate() {
    createUI();
}

void ProjectsView::draw(sf::RenderWindow& window) {
    for (auto& component : components) {
        component->draw(window);
    }
    while (!pendingOperations.empty()) {
        pendingOperations.front()();
        pendingOperations.pop();
    }
}

void ProjectsView::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    for (const auto& component : components) {
        component->handleEvent(event, window);
    }
    while (!pendingOperations.empty()) {
        pendingOperations.front()();
        pendingOperations.pop();
    }
}
