#include "ObjFile.hpp"

struct Color {
    float r{1.0f}, g{1.0f}, b{1.0f}, a{255.f};
};

void ObjFile::read(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open OBJ file: " + filename);
    }

    std::string line;
    Color currentColor;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream stream(line);
        std::string prefix;
        stream >> prefix;

        if (prefix == "v") {
            std::string vertexLine = line;
            float x, y, z;
            stream >> x >> y >> z;

            // Check for vertex colors
            if (!stream.eof()) {
                stream >> currentColor.r >> currentColor.g >> currentColor.b;
                vertexLine += " " + std::to_string(currentColor.r) + " "
                           + std::to_string(currentColor.g) + " "
                           + std::to_string(currentColor.b);
            }
            vertices.push_back(vertexLine);
        }
        else if (prefix == "f") {
            faces.push_back(line);
        }
        else if (prefix == "usemtl") {
            std::string materialName;
            stream >> materialName;
            if (!faces.empty()) {
                faces.back() += " # " + materialName;
            }
        }
    }
}

void ObjFile::write(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open OBJ file for writing: " + filename);
    }

    for (const auto& vertex : vertices) {
        file << vertex << "\n";
    }

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