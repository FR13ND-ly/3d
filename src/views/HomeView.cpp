#include "HomeView.hpp"
#include "ViewsManager.hpp"
#include "../utils/LanguageManager.hpp"
#include "../utils/WindowManager.hpp"

HomeView::HomeView() {
    if (!bgTexture.loadFromFile("../src/assets/images/bg.jpg")) {
        std::cerr << "Error loading image.png" << std::endl;
    }

    bgSprite.setTexture(bgTexture);

    bgSprite.setPosition(0.f, 0.f);
    sf::Vector2u windowSize = WindowManager::getInstance().getWindow().getSize();

    sf::Vector2u textureSize = bgTexture.getSize();

    float scaleX = static_cast<float>(windowSize.x) / textureSize.x;
    float scaleY = static_cast<float>(windowSize.y) / textureSize.y;

    bgSprite.setScale(scaleX, scaleY);
}

void HomeView::onActivate() {
    createUI();
}

void HomeView::createUI() {
    components.clear();
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    auto projectsButton = std::make_shared<Button>(
        sf::Vector2f(100, 700),
        sf::Vector2f(200, 50),
        languagePack["projects"]
    );

    projectsButton->setOnClick([this]() {
        ViewsManager::getInstance().switchTo("projects");
    });

    projectsButton->setIcon("projects");

    auto settingsButton = std::make_shared<Button>(
        sf::Vector2f(100, 770),
        sf::Vector2f(200, 50),
        languagePack["settings"]
    );


    settingsButton->setOnClick([this]() {
        ViewsManager::getInstance().switchTo("settings");
    });

    settingsButton->setIcon("settings");

    auto exitButton = std::make_shared<Button>(
        sf::Vector2f(100, 840),
        sf::Vector2f(200, 50),
        languagePack["exit"]
    );

    exitButton->setOnClick([this]() {
        sf::RenderWindow& window = WindowManager::getInstance().getWindow();
        window.close();
    });

    exitButton->setIcon("exit");

    this->addComponent(projectsButton);
    this->addComponent(settingsButton);
    this->addComponent(exitButton);
}

void HomeView::draw(sf::RenderWindow &window) {
    window.draw(bgSprite);
    for (auto& component : components) {
        component->draw(window);
    }
}

