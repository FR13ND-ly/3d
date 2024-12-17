#include "SettingsView.hpp"
#include "../../src/utils/files/Config.hpp"
#include "../../src/utils/files/FileManager.hpp"
#include "../../src/utils/LanguageManager.hpp"
#include "ViewsManager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
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
    auto projectsPathText = std::make_shared<Text>(
        sf::Vector2f(50, 50),
        20,
        static_cast<std::string>(languagePack["projectsPath"]) + ": " + config.getProjectsPath()
    );

    auto selectPathButton = std::make_shared<Button>(
        sf::Vector2f(500, 20),
        sf::Vector2f(250, 50),
        languagePack["change"]
    );

    selectPathButton->setOnClick([projectsPathText, languagePack]() {
        FileManager &fileManager = FileManager::getInstance();
        std::string newProjectsPath = fileManager.selectFolder();
        if (!newProjectsPath.empty()) {
            Config &config = Config::getInstance();
            config.updateProjectsPath(newProjectsPath);
            projectsPathText->setText(static_cast<std::string>(languagePack["projectsPath"]) + ": " + config.getProjectsPath());
        }
    });

    auto inertiaText = std::make_shared<Text>(
        sf::Vector2f(50, 100),
        20,
        static_cast<std::string>(languagePack["inertia"]) + ": "
    );

    auto inertiaCheckbox = std::make_shared<CheckBox>(
        sf::Vector2f(200, 100),
        20, config.getInertia()
    );

    inertiaCheckbox->setOnClick(static_cast<std::function<void(bool)>>([](bool checked) -> void {
        Config &config = Config::getInstance();
        config.updateInertia(checked);
    }));

    auto sensitivityText = std::make_shared<Text>(
        sf::Vector2f(50, 150),
        20,
        static_cast<std::string>(languagePack["sensitivity"]) + ": " + config.getFormattedSensitivity()
    );

    auto sensitivitySlider = std::make_shared<Slider>(
        sf::Vector2f(200, 160),
        sf::Vector2f(200, 20),
        0.1, 5, config.getSensitivity()
    );

    sensitivitySlider->setOnClick(
        static_cast<std::function<void(float)>>([this, sensitivityText, languagePack](float value) -> void {
            Config &config = Config::getInstance();
            config.updateSensitivity(value);
            sensitivityText->setText(static_cast<std::string>(languagePack["sensitivity"]) + ": " + config.getFormattedSensitivity());
        }));

    auto languageText = std::make_shared<Text>(
        sf::Vector2f(50, 200),
        20,
        static_cast<std::string>(languagePack["language"]) + ": "
    );

    std::vector<std::string> languages = LanguageManager::getInstance().getAllLanguages();
    int selectedLanguageIndex = LanguageManager::getInstance().getLanguageIndex();
    auto languageSelect = std::make_shared<Select>(
        sf::Vector2f(200, 200),
        sf::Vector2f(200, 200),
        languages, selectedLanguageIndex
    );

   languageSelect->setOnClick(std::function<void(float)>([this](float index) {
        LanguageManager::getInstance().setLanguageByIndex(static_cast<int>(index));
}));



    auto backButton = std::make_shared<Button>(
        sf::Vector2f(100, 400),
        sf::Vector2f(250, 50),
        languagePack["back"]
    );

    backButton->setOnClick([this]() {
        ViewsManager::getInstance().switchTo("home");
    });

    this->addComponent(backButton);
    this->addComponent(projectsPathText);
    this->addComponent(selectPathButton);
    this->addComponent(inertiaText);
    this->addComponent(inertiaCheckbox);
    this->addComponent(sensitivityText);
    this->addComponent(sensitivitySlider);
    this->addComponent(languageText);
    this->addComponent(languageSelect);
}
