#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <windows.h>
#include <shobjidl.h>
#include <codecvt>
#include <ctime>
#include <stdexcept>
#include <iostream>
#include <nlohmann/json.hpp>
#include <cpptoml.h>
#include "./FileType.hpp"
#include "./JsonFile.hpp"
#include "./TomlFile.hpp"
#include "Config.hpp"
#include "MtlFile.hpp"

enum class FileFormat {
    JSON,
    TOML,
    OBJ,
    PRJ
};

struct FileInfo {
    std::string fileName;
    std::string filePath;
    std::string lastUpdateDate;
    std::time_t lastModifiedTime;
};


class FileManager {
public:
    static FileManager& getInstance();

    std::unique_ptr<FileType> createFile(FileFormat format);
    std::unique_ptr<FileType> createFile(FileFormat format, const void* data);

    std::string selectFolder();
    std::string selectFile();
    std::vector<FileInfo> getFolderFiles(const std::string& folderPath);
    std::string saveAs();
    bool deleteFile(const std::string& filePath);
    bool createCopy(const std::string& sourceFilePath, const std::string& destinationFolderPath);
    std::string getExportObjPath();
    bool exportToObj(const std::string& objPath,
                     const std::vector<std::string>& vertices,
                     const std::vector<std::string>& faces,
                     const nlohmann::json& projectData);

private:
    FileManager() = default;
    FileManager(const FileManager&) = delete;
    FileManager& operator=(const FileManager&) = delete;
};

#endif
