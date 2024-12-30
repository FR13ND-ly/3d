#ifndef OBJFILE_HPP
#define OBJFILE_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "../RgbaColor.hpp"

class ObjFile {
public:
    void read(const std::string& filename);
    void write(const std::string& filename, const std::string& mtlFilename);

    const std::vector<std::string>& getVertices() const;
    const std::vector<std::string>& getFaces() const;
    const std::unordered_map<std::string, RgbaColor>& getMaterials() const;

    void setVertices(const std::vector<std::string>& vertices);
    void setFaces(const std::vector<std::string>& faces);
    void setMaterials(const std::unordered_map<std::string, RgbaColor>& materials);

private:
    std::vector<std::string> vertices;
    std::vector<std::string> faces;
    std::unordered_map<std::string, RgbaColor> materials;
    std::string mtlFilename;
};


#endif // OBJFILE_HPP
