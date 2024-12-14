#include "TomlFile.hpp"
#include <stdexcept>
#include <iostream>

TomlFile::TomlFile(const std::shared_ptr<cpptoml::table>& table) : tomlTable(table) {}

void TomlFile::read(const std::string& filename) {
    try {
        tomlTable = cpptoml::parse_file(filename);
    } catch (const std::exception& e) {
        std::cerr << "Error reading TOML file: " << e.what() << std::endl;
        throw;
    }
}

void TomlFile::write(const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open TOML file for writing: " + filename);
        }

        file << (*tomlTable);
    } catch (const std::exception& e) {
        std::cerr << "Error writing TOML file: " << e.what() << std::endl;
        throw;
    }
}

const std::shared_ptr<cpptoml::table>& TomlFile::getData() const {
    return tomlTable;
}

void TomlFile::setData(const std::shared_ptr<cpptoml::table>& table) {
    tomlTable = table;
}
