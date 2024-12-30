#include "MtlFile.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

void MtlFile::read(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open MTL file: " + filename);
    }

    std::string line;
    std::string currentMaterial;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream stream(line);
        std::string prefix;
        stream >> prefix;

        if (prefix == "newmtl") {
            stream >> currentMaterial;
        } else if (prefix == "Kd" && !currentMaterial.empty()) {
            RgbaColor color;
            stream >> color.r >> color.g >> color.b;
            color.a = 1.0f; // Default alpha value
            materials[currentMaterial] = color;
        } else if (prefix == "d" && !currentMaterial.empty()) {
            stream >> materials[currentMaterial].a;
        }
    }
}

void MtlFile::write(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open MTL file for writing: " + filename);
    }

    for (const auto& [materialName, color] : materials) {
        file << "newmtl " << materialName << "\n";
        file << "Ka 0.000 0.000 0.000\n";
        file << "Kd " << color.r << " " << color.g << " " << color.b << "\n";
        file << "Ks 0.000 0.000 0.000\n";
        file << "d " << color.a << "\n";
        file << "illum 2\n\n";
    }
}

const std::unordered_map<std::string, RgbaColor>& MtlFile::getMaterials() const {
    return materials;
}

void MtlFile::setMaterials(const std::unordered_map<std::string, RgbaColor>& materials) {
    this->materials = materials;
}
