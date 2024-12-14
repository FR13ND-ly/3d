#include "Config.hpp"
#include <iostream>
#include <cmath>
#include <cpptoml.h>  // Assuming you use the cpptoml library

Config& Config::getInstance() {
    static Config instance;
    return instance;
}

Config::Config() {
    try {
        auto& fileManager = FileManager::getInstance();

        if (!FileType::fileExists("config.toml")) {
            createDefaultConfig("config.toml");
        }

        auto tomlFile = fileManager.createFile(FileFormat::TOML);
        tomlFile->read("config.toml");

        auto* tomlFilePtr = dynamic_cast<TomlFile*>(tomlFile.get());
        if (!tomlFilePtr) {
            throw std::runtime_error("Failed to create TOML file object");
        }

        auto configTable = tomlFilePtr->getData();
        projectsPath = configTable->get_as<std::string>("projectsPath").value_or("");
        inertia = configTable->get_as<bool>("inertia").value_or(false);
        std::string sensitivityStr = configTable->get_as<std::string>("sensitivity").value_or("0.7");
        sensitivity = std::stof(sensitivityStr);
        language = configTable->get_as<std::string>("language").value_or("en");

        if (projectsPath.empty()) throw std::runtime_error("Missing 'projectsPath' in config.toml");
        if (sensitivity == 0.0) throw std::runtime_error("Missing or invalid 'sensitivity' in config.toml");
        if (language.empty()) throw std::runtime_error("Missing 'language' in config.toml");

    } catch (const std::exception& e) {
        std::cerr << "Error initializing Config: " << e.what() << std::endl;
        throw;
    }
}

Config::~Config() = default;

std::string Config::getProjectsPath() const { return projectsPath; }

bool Config::getInertia() const { return inertia; }

float Config::getSensitivity() const { return sensitivity; }

std::string Config::getFormattedSensitivity() const {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << sensitivity;
    return oss.str();
}

std::string Config::getLanguage() const { return language; }

void Config::updateProjectsPath(const std::string& newPath) {
    projectsPath = newPath;
    updateConfigFile();
}

void Config::updateInertia(bool newInertia) {
    inertia = newInertia;
    updateConfigFile();
}

void Config::updateSensitivity(float newSensitivity) {
    sensitivity = std::round(newSensitivity * 10.0f) / 10.0f;
    updateConfigFile();
}

void Config::updateLanguage(const std::string& newLanguage) {
    language = newLanguage;
    updateConfigFile();
}

void Config::updateConfigFile() {
    auto& fileManager = FileManager::getInstance();
    auto configTable = cpptoml::make_table();

    configTable->insert("projectsPath", projectsPath);
    configTable->insert("inertia", inertia);
    configTable->insert("sensitivity", getFormattedSensitivity());
    configTable->insert("language", language);

    auto tomlFile = fileManager.createFile(FileFormat::TOML, &configTable);
    tomlFile->write("config.toml");
}

void Config::createDefaultConfig(const std::string& filename) {
    auto& fileManager = FileManager::getInstance();

    auto configTable = cpptoml::make_table();
    configTable->insert("projectsPath", "../projects/");
    configTable->insert("inertia", true);
    configTable->insert("sensitivity", "2.0");
    configTable->insert("language", "en");

    auto tomlFile = fileManager.createFile(FileFormat::TOML, &configTable);
    tomlFile->write(filename);
}
