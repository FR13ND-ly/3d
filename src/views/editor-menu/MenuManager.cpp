#include "MenuManager.hpp"

#include <iostream>

#include "AddObjectsMenu.hpp"
#include "ObjectsMenu.hpp"
#include "../../core/ui/Surface.hpp"
#include "../ViewsManager.hpp"

MenuManager::MenuManager() {
    this->addView("addObjectsMenu", std::make_shared<AddObjectsMenu>());
    this->addView("objectsMenu", std::make_shared<ObjectsMenu>());
    this->switchTo("addObjectsMenu");
    createUI();
}

void MenuManager::createUI() {
    auto background = std::make_shared<Surface>(
        parentPosition, parentSize, sf::Color(100, 100, 100)
    );

    auto switchToAddMenu = std::make_shared<Button>(
    sf::Vector2f(parentPosition.x - 60, 10),
        sf::Vector2f(50, 50),
        "A"
    );

    switchToAddMenu->setOnClick([this]() {
        this->switchTo("addObjectsMenu");
    });

    auto switchToObjectsMenu = std::make_shared<Button>(
    sf::Vector2f(parentPosition.x - 60, 70),
        sf::Vector2f(50, 50),
        "O"
    );

    switchToObjectsMenu->setOnClick([this]() {
        this->switchTo("objectsMenu");
    });
    auto homeButton = std::make_shared<Button>(
        sf::Vector2f(1870, 1000),
        sf::Vector2f(50, 50),
        "H"
    );

    homeButton->setOnClick([this]() {
        ViewsManager::getInstance().switchTo("home");
    });
    components.emplace_back(background);
    components.emplace_back(switchToAddMenu);
    components.emplace_back(switchToObjectsMenu);
    components.emplace_back(homeButton);
}


MenuManager &MenuManager::getInstance() {
    static MenuManager instance;
    return instance;
}

void MenuManager::addView(const std::string &name, std::shared_ptr<Menu> menu) {
    menus[name] = menu;
}

void MenuManager::switchTo(const std::string &name) {
    if (currentMenu) {
        currentMenu->onDeactivate();
    }

    if (menus.find(name) != menus.end()) {
        currentMenu = menus[name];
        currentMenu->setParentPosition(parentPosition);
        currentMenu->onActivate();
    }
}


void MenuManager::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    for (const auto& component : components) {
        component->handleEvent(event, window);
    }
    if (currentMenu) {
        currentMenu->handleEvent(event, window);
    }
}

void MenuManager::draw(sf::RenderWindow &window) {
    for (const auto& component : components) {
        component->draw(window);
    }
    if (currentMenu) {
        currentMenu->draw(window);
    }
}

void MenuManager::setParentPosition(const sf::Vector2f &position) {
    parentPosition = position;
}

void MenuManager::setParentSize(const sf::Vector2f &size) {
    parentSize = size;
}
