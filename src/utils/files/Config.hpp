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
    static Config& getInstance(); // Singleton getter

    // Deleted copy constructor and assignment operator
    Config(const Config&) = delete;
    Config& operator=(const Config&) = delete;

    // Getter methods
    std::string getProjectsPath() const;
    bool getInertia() const;
    float getSensitivity() const;
    std::string getFormattedSensitivity() const;
    std::string getLanguage() const;

    // Update methods
    void updateProjectsPath(const std::string& newPath);
    void updateInertia(bool newInertia);
    void updateSensitivity(float newSensitivity);
    void updateLanguage(const std::string& newLanguage);

private:
    Config();  // Constructor
    ~Config(); // Destructor

    void updateConfigFile();
    void createDefaultConfig(const std::string& filename);

    std::string projectsPath;
    bool inertia;
    float sensitivity;
    std::string language;
};

#endif // CONFIG_HPP
