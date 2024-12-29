#include "LanguageManager.hpp"
#include "./files/Config.hpp"
#include "./files/FileManager.hpp"
#include "./files/JsonFile.hpp"

LanguageManager::LanguageManager(const std::string& languagePacksFile) {
    FileManager& fileManager = FileManager::getInstance();
    auto jsonFile = fileManager.createFile(FileFormat::JSON);
    jsonFile->read(languagePacksFile);

    auto jsonData = dynamic_cast<JsonFile*>(jsonFile.get())->getData();

    if (!jsonData.is_array()) {
        throw std::runtime_error("Invalid language packs format");
    }

    int index = 0;
    for (const auto& langPack : jsonData) {
        if (!langPack.contains("language") || !langPack.contains("showName") || !langPack.contains("words")) {
            throw std::runtime_error("Malformed language pack");
        }

        std::string lang = langPack["language"];
        languageIndexMap[lang] = index;
        indexLanguageMap[index] = lang;
        languagePacks.push_back(langPack);
        index++;
    }
}

LanguageManager& LanguageManager::getInstance() {
    static LanguageManager instance("../src/utils/LanguagePacks.json");
    return instance;
}

int LanguageManager::getLanguageIndex() const {
    std::string currentLanguage = Config::getInstance().getLanguage();
    if (languageIndexMap.find(currentLanguage) == languageIndexMap.end()) {
        throw std::runtime_error("Language not found: " + currentLanguage);
    }
    return languageIndexMap.at(currentLanguage);
}

void LanguageManager::setLanguageByIndex(int index) {
    if (indexLanguageMap.find(index) == indexLanguageMap.end()) {
        throw std::out_of_range("Invalid language index");
    }
    Config::getInstance().updateLanguage(indexLanguageMap.at(index));
}

const nlohmann::json& LanguageManager::getSelectedPack() const {
    int index = getLanguageIndex();
    return languagePacks.at(index)["words"];
}

std::vector<std::string> LanguageManager::getAllLanguages() const {
    std::vector<std::string> showNames;
    for (const auto& langPack : languagePacks) {
        if (langPack.contains("showName")) {
            showNames.push_back(langPack["showName"].get<std::string>());
        }
    }
    return showNames;
}
