#include "HomeView.hpp"
#include "ViewsManager.hpp"
#include "../utils/LanguageManager.hpp"
#include "../core/ui/Input.hpp"
#include "../core/ui/NumberInput.hpp"
#include "../core/ui/Expandable.hpp"

HomeView::HomeView() {}

void HomeView::onActivate() {
    createUI();
}

void HomeView::createUI() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    auto myButton = std::make_shared<Button>(
        sf::Vector2f(100, 100),
        sf::Vector2f(200, 50),
        languagePack["projects"]
    );

    auto settings = std::make_shared<Button>(
        sf::Vector2f(100, 200),
        sf::Vector2f(200, 50),
        languagePack["settings"]
    );

    myButton->setOnClick([this]() {
        ViewsManager::getInstance().switchTo("projects");
    });

    settings->setOnClick([this]() {
        ViewsManager::getInstance().switchTo("settings");
    });

    auto myText = std::make_shared<Text>(
        sf::Vector2f(400, 100),
        20,
        "Home Page"
    );
    auto expandable = std::make_shared<Expandable>(
        sf::Vector2f(1000, 100),
        sf::Vector2f(200, 50),
        "Expandable"
    );

    expandable->addContent(std::make_shared<Button>(
        sf::Vector2f(1000, 200),
        sf::Vector2f(200, 50),
        languagePack["settings"]
    ));
    expandable->addContent(std::make_shared<Button>(
    sf::Vector2f(1000, 200),
    sf::Vector2f(200, 50),
    languagePack["settings"]
));
    expandable->addContent(std::make_shared<Button>(
    sf::Vector2f(1000, 200),
    sf::Vector2f(200, 50),
    languagePack["settings"]
));


    // expandable.setOnClick([](bool isExpanded) {
    //     std::cout << "Expanded state: " << (isExpanded ? "Opened" : "Closed") << std::endl;
    // });


    this->addComponent(myText);
    this->addComponent(myButton);
    this->addComponent(settings);
    // this->addComponent(expandable);
}
