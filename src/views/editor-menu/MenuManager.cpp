#include "MenuManager.hpp"

#include "AddObjectsMenu.hpp"
#include "ObjectsMenu.hpp"
#include "FacesMenu.hpp"
#include "VerticesMenu.hpp"
#include "ProjectSettings.hpp"
#include "../../core/ui/Surface.hpp"
#include "../ViewsManager.hpp"
#include "../../utils/LanguageManager.hpp"

MenuManager::MenuManager() {
    this->addView("addObjectsMenu", std::make_shared<AddObjectsMenu>());
    this->addView("objectsMenu", std::make_shared<ObjectsMenu>());
    this->addView("facesMenu", std::make_shared<FacesMenu>());
    this->addView("verticesMenu", std::make_shared<VerticesMenu>());
    this->addView("projectSettings", std::make_shared<ProjectSettings>());
}

void MenuManager::onActivate() {
    this->switchTo("addObjectsMenu");
    createUI();
}

void MenuManager::onDeactivate() {
    for (auto& menu: menus) {
        menu.second->onDeactivate();
    }
}

void MenuManager::createUI() {
    components.clear();
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    auto background = std::make_shared<Surface>(
        parentPosition, parentSize, sf::Color(100, 100, 100)
    );

    auto switchToAddMenu = std::make_shared<Button>(
    sf::Vector2f(parentPosition.x - 60, 10),
        sf::Vector2f(50, 50),
        "",
        sf::Color(255, 255, 100)
    );

    switchToAddMenu->setIcon("add");

    switchToAddMenu->setOnClick([this]() {
        this->switchTo("addObjectsMenu");
    });

    auto switchToObjectsMenu = std::make_shared<Button>(
    sf::Vector2f(parentPosition.x - 60, 70),
        sf::Vector2f(50, 50),
        ""
    );

    switchToObjectsMenu->setIcon("objects");

    switchToObjectsMenu->setOnClick([this]() {
        this->switchTo("objectsMenu");
    });

    auto switchToFacesMenu = std::make_shared<Button>(
sf::Vector2f(parentPosition.x - 60, 130),
    sf::Vector2f(50, 50),
    ""
    );

    switchToFacesMenu->setIcon("faces");

    switchToFacesMenu->setOnClick([this]() {
        this->switchTo("facesMenu");
    });

    auto switchToVerticesMenu = std::make_shared<Button>(
        sf::Vector2f(parentPosition.x - 60, 190),
        sf::Vector2f(50, 50),
        ""
    );

    switchToVerticesMenu->setIcon("vertices");

    switchToVerticesMenu->setOnClick([this]() {
        this->switchTo("verticesMenu");
    });

    auto projectsSettings = std::make_shared<Button>(
    sf::Vector2f(parentPosition.x - 60, 250),
        sf::Vector2f(50, 50),
        ""
        );

    projectsSettings->setOnClick([this]() {
        this->switchTo("projectSettings");
    });

    projectsSettings->setIcon("edit");

    auto homeButton = std::make_shared<Button>(
        sf::Vector2f(10, 10),
        sf::Vector2f(120, 50),
        languagePack["Home"]
    );

    homeButton->setIcon("home");

    homeButton->setOnClick([this]() {
        ViewsManager::getInstance().switchTo("home");
    });


    components.emplace_back(background);
    components.emplace_back(switchToAddMenu);
    components.emplace_back(switchToObjectsMenu);
    components.emplace_back(switchToFacesMenu);
    components.emplace_back(switchToVerticesMenu);
    components.emplace_back(projectsSettings);
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
    if (components.size() > 6) {
        for (int i = 1; i < 7; i++) {
            auto btn = std::dynamic_pointer_cast<Button>(components[i]);
            if (btn) {
                btn->setColor(sf::Color::White);
            }
        }
    }

    if (menus.find(name) != menus.end()) {
        currentMenu = menus[name];
        currentMenu->setParentPosition(parentPosition);
        currentMenu->onActivate();
        if (components.size() < 5) return;
        if (name == "addObjectsMenu") {
            auto btn = std::dynamic_pointer_cast<Button>(components[1]);
            if (btn) {
                btn->setColor(sf::Color(255, 255, 100));
            }
        }
        if (name == "objectsMenu") {
            auto btn = std::dynamic_pointer_cast<Button>(components[2]);
            if (btn) {
                btn->setColor(sf::Color(255, 255, 100));
            }
        }
        if (name == "facesMenu") {
            auto btn = std::dynamic_pointer_cast<Button>(components[3]);
            if (btn) {
                btn->setColor(sf::Color(255, 255, 100));
            }
        }
        if (name == "verticesMenu") {
            auto btn = std::dynamic_pointer_cast<Button>(components[4]);
            if (btn) {
                btn->setColor(sf::Color(255, 255, 100));
            }
        }
        if (name == "projectSettings") {
            auto btn = std::dynamic_pointer_cast<Button>(components[5]);
            if (btn) {
                btn->setColor(sf::Color(255, 255, 100));
            }
        }

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

float MenuManager::getHeight() const {
    return parentSize.y;
}

sf::Vector2f MenuManager::getPosition() const {
    return parentPosition;
}

void MenuManager::setPosition(const sf::Vector2f& position) {
    parentPosition = position;
    if (currentMenu) {
        currentMenu->setParentPosition(position);
    }
}