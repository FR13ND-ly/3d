#ifndef FILE_TYPE_HPP
#define FILE_TYPE_HPP

#include <string>
#include <fstream>

class FileType {
public:
    virtual ~FileType() = default;

    // Pure virtual functions for reading and writing files
    virtual void read(const std::string& filename) = 0;
    virtual void write(const std::string& filename) = 0;

    // Static function to check if a file exists
    static bool fileExists(const std::string& filename);

protected:
    // Protected default constructor
    FileType() = default;
};

#endif // FILE_TYPE_HPP
