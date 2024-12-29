#include "AddObjectsMenu.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../utils/WindowManager.hpp"
#include "../../utils/LanguageManager.hpp"
#include "../../core/ui/Text.hpp"
#include "../../core/ui/Button.hpp"
#include "../../core/ui/Snackbar.hpp"

AddObjectsMenu::AddObjectsMenu() {
}

void AddObjectsMenu::createUI() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();

    components.clear();

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 20),
        30,
        languagePack["Add objects"]
    );

    auto addTriangleButton = std::make_shared<Button>(
           sf::Vector2f(parentPosition.x + 20, 80),
           sf::Vector2f(310, 50),
           languagePack["triangle"]
       );

    addTriangleButton->setIcon("triangle");

    addTriangleButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("triangle");
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["object_added"]);
    });

    auto addCubeButton = std::make_shared<Button>(
        sf::Vector2f(parentPosition.x + 20, 150),
        sf::Vector2f(310, 50),
        languagePack["Cube"]
    );

    addCubeButton->setIcon("cube");

    addCubeButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("cube");
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["object_added"]);
    });

    auto addSphereButton = std::make_shared<Button>(
         sf::Vector2f(parentPosition.x + 20, 220),
         sf::Vector2f(310, 50),
         languagePack["Sphere"]
     );

    addSphereButton->setIcon("sphere");

    addSphereButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("sphere");
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["object_added"]);
    });

    auto addCylinderButton = std::make_shared<Button>(
         sf::Vector2f(parentPosition.x + 20, 290),
         sf::Vector2f(310, 50),
         languagePack["Cylinder"]
     );

    addCylinderButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("cylinder");
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["object_added"]);
    });

    addCylinderButton->setIcon("cylinder");

    auto addConeButton = std::make_shared<Button>(
         sf::Vector2f(parentPosition.x + 20, 360),
         sf::Vector2f(310, 50),
          languagePack["Cone"]
     );

    addConeButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("cone");
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["object_added"]);
    });

    addConeButton->setIcon("cone");

    auto addTorusButton = std::make_shared<Button>(
         sf::Vector2f(parentPosition.x + 20, 430),
         sf::Vector2f(310, 50),
         languagePack["torus"]
     );

    addTorusButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("torus");
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["object_added"]);
    });

    addTorusButton->setIcon("torus");

    auto importButton = std::make_shared<Button>(
         sf::Vector2f(parentPosition.x + 20, 500),
         sf::Vector2f(310, 50),
         languagePack["import"]
     );

    importButton->setIcon("import");

    importButton->setOnClick([]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        Scene& scene = Scene::getInstance(window);
        scene.addObject("import");
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["file_imported"]);
    });

    this->addComponent(title);
    this->addComponent(addTriangleButton);
    this->addComponent(addCubeButton);
    this->addComponent(addSphereButton);
    this->addComponent(addCylinderButton);
    this->addComponent(addConeButton);
    this->addComponent(addTorusButton);
    this->addComponent(importButton);
}

void AddObjectsMenu::onActivate() {
    createUI();
}

void AddObjectsMenu::onDeactivate() {
    components.clear();
}


