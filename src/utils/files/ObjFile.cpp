#include "ObjFile.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <filesystem>

#include "MtlFile.hpp"

namespace fs = std::filesystem;

void ObjFile::read(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open OBJ file: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream stream(line);
        std::string prefix;
        stream >> prefix;

        if (prefix == "v") {
            vertices.push_back(line);
        } else if (prefix == "f") {
            faces.push_back(line);
        } else if (prefix == "mtllib") {
            stream >> mtlFilename;
        }
    }
}

void ObjFile::write(const std::string& filename, const std::string& mtlFilename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open OBJ file for writing: " + filename);
    }

    // Write material library reference
    file << "mtllib " << mtlFilename << "\n";

    // Write vertices
    for (const auto& vertex : vertices) {
        file << vertex << "\n";
    }
    file << "\n";

    // Write faces with material references
    for (const auto& face : faces) {
        file << face << "\n";
    }
}

const std::vector<std::string>& ObjFile::getVertices() const {
    return vertices;
}

const std::vector<std::string>& ObjFile::getFaces() const {
    return faces;
}

void ObjFile::setVertices(const std::vector<std::string>& vertices) {
    this->vertices = vertices;
}

void ObjFile::setFaces(const std::vector<std::string>& faces) {
    this->faces = faces;
}
