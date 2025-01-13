#ifndef FILE_TYPE_HPP
#define FILE_TYPE_HPP

#include <string>
#include <fstream>

class FileType {
public:
    virtual ~FileType() = default;

    virtual void read(const std::string& filename) = 0;
    virtual void write(const std::string& filename) = 0;

    static bool fileExists(const std::string& filename);

protected:
    FileType() = default;
};

#endif