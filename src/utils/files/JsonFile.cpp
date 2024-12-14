#include "JsonFile.hpp"
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <iostream>

JsonFile::JsonFile(const nlohmann::json& data) : jsonData(data) {}

void JsonFile::read(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open JSON file: " + filename);
        }

        file >> jsonData;
    } catch (const std::exception& e) {
        std::cerr << "Error reading JSON file: " << e.what() << std::endl;
        throw;
    }
}

void JsonFile::write(const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open JSON file for writing: " + filename);
        }

        file << jsonData.dump(4);
    } catch (const std::exception& e) {
        std::cerr << "Error writing JSON file: " << e.what() << std::endl;
        throw;
    }
}

const nlohmann::json& JsonFile::getData() const {
    return jsonData;
}

void JsonFile::setData(const nlohmann::json& data) {
    jsonData = data;
}
