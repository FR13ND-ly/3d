#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>
#include <memory>
#include <windows.h>
#include <shobjidl.h>
#include "./FileType.hpp"
#include "./JsonFile.hpp"
#include "./TomlFile.hpp"

enum class FileFormat {
    JSON,
    TOML
};

class FileManager {
public:
    // Singleton instance
    static FileManager& getInstance();

    // Delete copy constructor and assignment operator
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;

    // Factory method to create file type objects
    std::unique_ptr<FileType> createFile(FileFormat format);

    // Factory method to create file type objects with initial data
    std::unique_ptr<FileType> createFile(FileFormat format, const void* data);

    // Method to open folder selection dialog
    std::string selectFolder();

private:
    // Private constructor for singleton pattern
    FileManager() = default;
};

#endif // FILE_MANAGER_HPP
