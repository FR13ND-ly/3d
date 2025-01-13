#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include "FileManager.hpp"
#include <cpptoml.h>

class Config {
public:
    static Config& getInstance();

    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    std::string getProjectsPath() const;
    bool getInertia() const;
    float getSensitivity() const;
    std::string getFormattedSensitivity() const;
    std::string getLanguage() const;

    void updateProjectsPath(const std::string& newPath);
    void updateInertia(bool newInertia);
    void updateSensitivity(float newSensitivity);
    void updateLanguage(const std::string& newLanguage);

private:
    Config();
    ~Config();

    void updateConfigFile();
    void createDefaultConfig(const std::string& filename);

    std::string projectsPath;
    bool inertia;
    float sensitivity;
    std::string language;
};

#endif
