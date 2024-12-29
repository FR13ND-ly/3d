#include "EditorView.hpp"
#include <memory>
#include "ViewsManager.hpp"
#include "../core/feature/ProjectsManager.hpp"
#include "./editor-menu/MenuManager.hpp"
#include "../utils/WindowManager.hpp"

EditorView::EditorView() {
    editorMenu = std::make_shared<MenuManager>();
}

void EditorView::clearComponents() {
    components.clear();
}

void EditorView::onActivate() {
    createUI();
}

void EditorView::onDeactivate() {
    editorMenu->onDeactivate();
}

void EditorView::handleEvent(const sf::Event &event, sf::RenderWindow &window) {
    for (const auto& component : components) {
            component->handleEvent(event, window);
    }
    if (event.type == sf::Event::KeyPressed && event.key.control && event.key.code == sf::Keyboard::S) {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        ProjectsManager& projectsManager = ProjectsManager::getInstance();
        projectsManager.updateProject(scene);
    }
}


void EditorView::createUI() {
    components.clear();
    editorMenu->onActivate();
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& sceneInst = Scene::getInstance(window);
    this->addComponent(std::shared_ptr<Component>(&sceneInst, [](Component*){}));
    this->addComponent(editorMenu);
}
