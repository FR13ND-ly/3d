#include "SettingsView.hpp"
#include "../../src/utils/files/Config.hpp"
#include "../../src/utils/files/FileManager.hpp"
#include "../../src/utils/LanguageManager.hpp"
#include "ViewsManager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include "../core/ui/Surface.hpp"

#include "../core/ui/Component.hpp"
#include "../core/ui/Button.hpp"
#include "../core/ui/Text.hpp"
#include "../core/ui/Slider.hpp"
#include "../core/ui/Checkbox.hpp"
#include "../core/ui/Select.hpp"

SettingsView::SettingsView() {}

void SettingsView::onActivate() {
    createUI();
}


void SettingsView::createUI() {
    components.clear();
    Config &config = Config::getInstance();
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
        languagePack["settings"],
        sf::Color::White
    );

    auto authorsDividerStart = std::make_shared<Surface>(
        sf::Vector2f(20, 110),
        sf::Vector2f(1880, 1),
        sf::Color(50, 50, 50)
    );

    auto authorsTitle = std::make_shared<Text>(
        sf::Vector2f(50, 140),
        25,
        languagePack["authors"]
    );
    authorsTitle->setBold();

    auto authorText = std::make_shared<Text>(
        sf::Vector2f(50, 190),
        20,
        "Robert Ciobanu, M.A.G."
    );


    auto authorsDividerEnd = std::make_shared<Surface>(
        sf::Vector2f(20, 260),
        sf::Vector2f(1880, 1),
        sf::Color(50, 50, 50)
    );

    auto projectsPathTitle = std::make_shared<Text>(
        sf::Vector2f(50, 290),
        25,
        static_cast<std::string>(languagePack["projectsPath"]) + ": "
    );
    projectsPathTitle->setBold();

    auto projectsPathText = std::make_shared<Text>(
        sf::Vector2f(50, 340),
        20,
        config.getProjectsPath()
    );

    auto selectPathButton = std::make_shared<Button>(
        sf::Vector2f(projectsPathText->getWidth() + 70, 335),
        sf::Vector2f(100, 40),
        languagePack["change"]
    );
    selectPathButton->setIcon("folder");

    selectPathButton->setOnClick([this]() {
        FileManager &fileManager = FileManager::getInstance();
        std::string newProjectsPath = fileManager.selectFolder();
        if (!newProjectsPath.empty()) {
            Config &config = Config::getInstance();
            config.updateProjectsPath(newProjectsPath);
            pendingOperations.push([this]() {
               createUI();
           });
        }
    });

    auto prjPathDividerEnd = std::make_shared<Surface>(
        sf::Vector2f(20, 410),
        sf::Vector2f(1880, 1),
        sf::Color(50, 50, 50)
    );


    auto inertiaText = std::make_shared<Text>(
        sf::Vector2f(50, 440),
        25,
        static_cast<std::string>(languagePack["inertia"]) + ": "
    );

    inertiaText->setBold();

    auto inertiaCheckbox = std::make_shared<CheckBox>(
        sf::Vector2f(50, 490),
        20, config.getInertia()
    );

    inertiaCheckbox->setOnClick(static_cast<std::function<void(bool)>>([](bool checked) -> void {
        Config &config = Config::getInstance();
        config.updateInertia(checked);
    }));

    auto inertiaDividerEnd = std::make_shared<Surface>(
        sf::Vector2f(20, 560),
        sf::Vector2f(1880, 1),
        sf::Color(50, 50, 50)
    );

    auto sensitivityText = std::make_shared<Text>(
        sf::Vector2f(50, 590),
        25,
        static_cast<std::string>(languagePack["sensitivity"]) + ": " + config.getFormattedSensitivity()
    );
    sensitivityText->setBold();

    auto sensitivitySlider = std::make_shared<Slider>(
        sf::Vector2f(50, 640),
        sf::Vector2f(200, 20),
        0.1, 5, config.getSensitivity()
    );

    sensitivitySlider->setOnClick(
        static_cast<std::function<void(float)>>([this, sensitivityText, languagePack](float value) -> void {
            Config &config = Config::getInstance();
            config.updateSensitivity(value);
            sensitivityText->setText(static_cast<std::string>(languagePack["sensitivity"]) + ": " + config.getFormattedSensitivity());
        }));

    auto sensitivityDividerEnd = std::make_shared<Surface>(
        sf::Vector2f(20, 710),
        sf::Vector2f(1880, 1),
        sf::Color(50, 50, 50)
    );

    auto languageText = std::make_shared<Text>(
        sf::Vector2f(50, 740),
        25,
        languagePack["language"]
    );
    languageText->setBold();

    std::vector<std::string> languages = LanguageManager::getInstance().getAllLanguages();
    int selectedLanguageIndex = LanguageManager::getInstance().getLanguageIndex();
    auto languageSelect = std::make_shared<Select>(
        sf::Vector2f(50, 780),
        sf::Vector2f(200, 200),
        languages, selectedLanguageIndex
    );

   languageSelect->setOnClick(std::function<void(float)>([this](float index) {
       LanguageManager::getInstance().setLanguageByIndex(static_cast<int>(index));
       pendingOperations.push([this]() {
           createUI();
       });
    }));

    auto languageDividerEnd = std::make_shared<Surface>(
        sf::Vector2f(20, 860),
        sf::Vector2f(1880, 1),
        sf::Color(50, 50, 50)
    );

    this->addComponent(authorsDividerStart);
    this->addComponent(authorsDividerEnd);
    this->addComponent(prjPathDividerEnd);
    this->addComponent(inertiaDividerEnd);
    this->addComponent(sensitivityDividerEnd);
    this->addComponent(languageDividerEnd);

    this->addComponent(headerSurface);
    this->addComponent(backButton);
    this->addComponent(title);
    this->addComponent(authorsTitle);
    this->addComponent(authorText);
    this->addComponent(projectsPathTitle);
    this->addComponent(projectsPathText);
    this->addComponent(selectPathButton);
    this->addComponent(inertiaText);
    this->addComponent(inertiaCheckbox);
    this->addComponent(sensitivityText);
    this->addComponent(sensitivitySlider);
    this->addComponent(languageText);
    this->addComponent(languageSelect);

}

void SettingsView::draw(sf::RenderWindow& window) {
    for (auto& component : components) {
        component->draw(window);
    }
    while (!pendingOperations.empty()) {
        pendingOperations.front()();
        pendingOperations.pop();
    }
}

void SettingsView::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    for (const auto& component : components) {
        component->handleEvent(event, window);
    }
    while (!pendingOperations.empty()) {
        pendingOperations.front()();
        pendingOperations.pop();
    }
}
