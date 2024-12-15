#include "EditorView.hpp"
#include <memory>
#include "ViewsManager.hpp"
#include "./editor-menu/MenuManager.hpp"

EditorView::EditorView() {
    initializeScene();
}

void EditorView::clearComponents() {
    components.clear();
}
void EditorView::onActivate() {

    createUI();
}
void EditorView::onDeactivate() {

}

void EditorView::initializeScene() {
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& sceneInst = Scene::getInstance(window);
    // auto scene = std::make_shared<Scene>(window);
    this->addComponent(std::shared_ptr<Component>(&sceneInst, [](Component*){}));
}

void EditorView::createUI() {
    auto myButton = std::make_shared<Button>(
        sf::Vector2f(50, 100),
        sf::Vector2f(200, 50),
        "Home"
    );

    auto Reset = std::make_shared<Button>(
        sf::Vector2f(50, 400),
        sf::Vector2f(200, 50),
        "Reset"
    );

    auto sphereButton = std::make_shared<Button>(
        sf::Vector2f(50, 300),
        sf::Vector2f(200, 50),
        "Sphere"
    );

    auto cubeButton = std::make_shared<Button>(
        sf::Vector2f(50, 200),
        sf::Vector2f(200, 50),
        "Cube"
    );

    cubeButton->setOnClick([this]() {
        // this->scene->addObject(std::make_shared<Cube>(1.f));
        this->scene->addObject("cube");
    });

    sphereButton->setOnClick([this]() {
        this->scene->addObject("sphere");
    });

    myButton->setOnClick([this]() {
        this->clearComponents();
        initializeScene();
        createUI();
        ViewsManager::getInstance().switchTo("home");
    });

    Reset->setOnClick([this]() {
        this->clearComponents();
        initializeScene();
        createUI();
    });

    auto editorMenu = std::make_shared<MenuManager>();

    // this->addComponent(myButton);
    // this->addComponent(Reset);
    // this->addComponent(sphereButton);
    // this->addComponent(cubeButton);
    this->addComponent(editorMenu);
}
