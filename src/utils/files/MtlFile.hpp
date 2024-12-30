#ifndef MTLFILE_HPP
#define MTLFILE_HPP

#include <string>
#include <unordered_map>
#include "../RgbaColor.hpp"

class MtlFile {
public:
    void read(const std::string& filename);
    void write(const std::string& filename);

    const std::unordered_map<std::string, RgbaColor>& getMaterials() const;
    void setMaterials(const std::unordered_map<std::string, RgbaColor>& materials);

private:
    std::unordered_map<std::string, RgbaColor> materials;
};

#endif // MTLFILE_HPP
