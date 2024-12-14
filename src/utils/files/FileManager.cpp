#include "FileManager.hpp"
#include <stdexcept>
#include <iostream>
#include <codecvt>
#include <nlohmann/json.hpp>
#include <cpptoml.h>

FileManager& FileManager::getInstance() {
    static FileManager instance;
    return instance;
}

std::unique_ptr<FileType> FileManager::createFile(FileFormat format) {
    switch (format) {
        case FileFormat::JSON:
            return std::make_unique<JsonFile>();
        case FileFormat::TOML:
            return std::make_unique<TomlFile>();
        default:
            throw std::runtime_error("Unsupported file format");
    }
}

std::unique_ptr<FileType> FileManager::createFile(FileFormat format, const void* data) {
    switch (format) {
        case FileFormat::JSON: {
            const nlohmann::json* jsonData = static_cast<const nlohmann::json*>(data);
            return std::make_unique<JsonFile>(*jsonData);
        }
        case FileFormat::TOML: {
            const std::shared_ptr<cpptoml::table>* tomlTable =
                static_cast<const std::shared_ptr<cpptoml::table>*>(data);
            return std::make_unique<TomlFile>(*tomlTable);
        }
        default:
            throw std::runtime_error("Unsupported file format");
    }
}

std::string FileManager::selectFolder() {
    std::string folderPath = "";

    // Initialize COM library
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileDialog *pFileDialog = nullptr;

        // Create the FileOpenDialog object
        hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
        if (SUCCEEDED(hr)) {
            // Set options to select folders
            DWORD options;
            pFileDialog->GetOptions(&options);
            pFileDialog->SetOptions(options | FOS_PICKFOLDERS);

            // Show the folder selector dialog
            hr = pFileDialog->Show(nullptr);
            if (SUCCEEDED(hr)) {
                IShellItem *pItem = nullptr;
                hr = pFileDialog->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFolderPath = nullptr;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
                    if (SUCCEEDED(hr)) {
                        // Convert wide string (PWSTR) to narrow string (std::string)
                        std::wstring wstrFolderPath(pszFolderPath);
                        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                        folderPath = converter.to_bytes(wstrFolderPath);

                        // Free the memory allocated for pszFolderPath
                        CoTaskMemFree(pszFolderPath);
                    }
                    pItem->Release();
                }
            }
            pFileDialog->Release();
        }
        CoUninitialize();
    }

    if (folderPath.empty()) {
        std::cout << "No folder selected or an error occurred." << std::endl;
    }
    return folderPath;
}
