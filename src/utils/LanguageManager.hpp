#ifndef LANGUAGE_MANAGER_HPP
#define LANGUAGE_MANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

class LanguageManager {
public:
    static LanguageManager& getInstance();

    LanguageManager(const LanguageManager&) = delete;
    LanguageManager& operator=(const LanguageManager&) = delete;

    int getLanguageIndex() const;

    void setLanguageByIndex(int index);

    nlohmann::json getSelectedPack() const;

    std::vector<std::string> getAllLanguages() const;

private:
    LanguageManager(const std::string& languagePacksFile);

    std::unordered_map<std::string, int> languageIndexMap;
    std::unordered_map<int, std::string> indexLanguageMap;
    std::vector<nlohmann::json> languagePacks;
};

#endif
