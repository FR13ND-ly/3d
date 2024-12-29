#include "PrjFile.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>

void PrjFile::read(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for reading: " + filename);
        }

        file >> projectData;
    } catch (const std::exception& e) {
        std::cerr << "Error reading project file: " << e.what() << std::endl;
        throw;
    }
}

void PrjFile::write(const std::string& filename) {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for writing: " + filename);
        }

        file << projectData.dump(4);
    } catch (const std::exception& e) {
        std::cerr << "Error writing project file: " << e.what() << std::endl;
        throw;
    }
}

void PrjFile::setCamera(Vector3 position, float yaw, float pitch, Vector3 orbitCenter) {
    projectData["camera"]["position"] = {position.x, position.y, position.z};
    projectData["camera"]["yaw"] = yaw;
    projectData["camera"]["pitch"] = pitch;
    projectData["camera"]["orbitCenter"] = {orbitCenter.x, orbitCenter.y, orbitCenter.z};
}

void PrjFile::addObject(const nlohmann::json& object) {
    if (!object.contains("position") || !object.contains("rotation") || !object.contains("scale") ||
        !object.contains("vertices") || !object.contains("faces")) {
        throw std::invalid_argument("Object JSON must contain position, rotation, scale, vertices, and faces.");
        }

    projectData["objects"].push_back(object);
}

const nlohmann::json& PrjFile::getCamera() const {
    return projectData["camera"];
}

const std::vector<nlohmann::json>& PrjFile::getObjects() const {
    return projectData["objects"].get<std::vector<nlohmann::json>>();
}
