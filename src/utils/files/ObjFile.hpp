#ifndef OBJFILE_HPP
#define OBJFILE_HPP

#include "FileType.hpp"
#include <vector>
#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>

class ObjFile : public FileType {
public:
    ObjFile() = default;

    void read(const std::string& filename) override;
    void write(const std::string& filename) override;

    const std::vector<std::string>& getVertices() const;
    const std::vector<std::string>& getFaces() const;

    void setVertices(const std::vector<std::string>& vertices);
    void setFaces(const std::vector<std::string>& faces);

private:
    std::vector<std::string> vertices;
    std::vector<std::string> faces;
};

#endif