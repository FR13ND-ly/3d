#include "FileType.hpp"

bool FileType::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}
