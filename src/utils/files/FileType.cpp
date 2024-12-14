#include "FileType.hpp"

// Static function to check if a file exists
bool FileType::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good();
}
