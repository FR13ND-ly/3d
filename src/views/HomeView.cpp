#include "HomeView.hpp"
#include "ViewsManager.hpp"
#include "../utils/LanguageManager.hpp"
#include "../core/ui/Input.hpp"
#include "../core/ui/NumberInput.hpp"

HomeView::HomeView() {
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
    auto numberInput = std::make_shared<NumberInput>(
        sf::Vector2f(300, 200),
        sf::Vector2f(200, 50),
        "Position X"
    );
    numberInput->setValue(2);

    numberInput->setOnClick(std::function<void(float)>([this](float value) {
        std::cout << value << std::endl;
    }));


    auto myText = std::make_shared<Text>(
        sf::Vector2f(400, 100),
        20,
        "Home Page"
    );

    this->addComponent(myText);
    this->addComponent(myButton);
    this->addComponent(settings);
    this->addComponent(numberInput);
}
