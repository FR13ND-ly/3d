#include "AddObjectsMenu.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../utils/WindowManager.hpp"

AddObjectsMenu::AddObjectsMenu() {
}

void AddObjectsMenu::createUI() {
    components.clear();

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 50, 50),
        20,
        "Add objects"
    );

    auto addCubeButton = std::make_shared<Button>(
        sf::Vector2f(parentPosition.x + 50, 100),
        sf::Vector2f(200, 50),
        "Cube"
    );

    addCubeButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("cube");
    });

    auto addSphereButton = std::make_shared<Button>(
         sf::Vector2f(parentPosition.x + 50, 170),
         sf::Vector2f(200, 50),
         "Sphere"
     );

    addSphereButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("sphere");
    });

    auto addCylinderButton = std::make_shared<Button>(
         sf::Vector2f(parentPosition.x + 50, 240),
         sf::Vector2f(200, 50),
         "Cylinder"
     );

    addCylinderButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("cylinder");
    });

    auto addConeButton = std::make_shared<Button>(
         sf::Vector2f(parentPosition.x + 50, 310),
         sf::Vector2f(200, 50),
         "Cone"
     );

    addConeButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("cone");
    });

    this->addComponent(title);
    this->addComponent(addCubeButton);
    this->addComponent(addSphereButton);
    this->addComponent(addCylinderButton);
    this->addComponent(addConeButton);
}

void AddObjectsMenu::onActivate() {
    createUI();
}

void AddObjectsMenu::onDeactivate() {
    components.clear();
}


