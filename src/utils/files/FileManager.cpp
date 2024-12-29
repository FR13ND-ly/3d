#include "FileManager.hpp"

#include "ObjFile.hpp"
#include "../LanguageManager.hpp"
#include "../../core/ui/Snackbar.hpp"

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
            auto languagePack = LanguageManager::getInstance().getSelectedPack();
            Snackbar::getInstance().addMessage(languagePack["unsupported_file_format"]);
    }
}

std::string FileManager::selectFolder() {
    std::string folderPath = "";

    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileDialog *pFileDialog = nullptr;

        hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
        if (SUCCEEDED(hr)) {
            DWORD options;
            pFileDialog->GetOptions(&options);
            pFileDialog->SetOptions(options | FOS_PICKFOLDERS);

            hr = pFileDialog->Show(nullptr);
            if (SUCCEEDED(hr)) {
                IShellItem *pItem = nullptr;
                hr = pFileDialog->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFolderPath = nullptr;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFolderPath);
                    if (SUCCEEDED(hr)) {
                        std::wstring wstrFolderPath(pszFolderPath);
                        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                        folderPath = converter.to_bytes(wstrFolderPath);
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
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["no_folder_selected"]);
    }
    return folderPath;
}

std::string FileManager::selectFile() {
    std::string filePath = "";

    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (SUCCEEDED(hr)) {
        IFileDialog* pFileDialog = nullptr;

        hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
        if (SUCCEEDED(hr)) {
            hr = pFileDialog->Show(nullptr);
            if (SUCCEEDED(hr)) {
                IShellItem* pItem = nullptr;
                hr = pFileDialog->GetResult(&pItem);
                if (SUCCEEDED(hr)) {
                    PWSTR pszFilePath = nullptr;
                    hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                    if (SUCCEEDED(hr)) {
                        std::wstring wstrFilePath(pszFilePath);
                        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
                        filePath = converter.to_bytes(wstrFilePath);
                        CoTaskMemFree(pszFilePath);
                    }
                    pItem->Release();
                }
            }
            pFileDialog->Release();
        }
        CoUninitialize();
    }

    if (filePath.empty()) {
        std::cout << "No file selected or an error occurred." << std::endl;
    }
    return filePath;
}

std::vector<FileInfo> FileManager::getFolderFiles(const std::string& folderPath) {
    namespace fs = std::filesystem;
    std::vector<FileInfo> files;

    try {
        if (!fs::exists(folderPath)) {
            fs::create_directories(folderPath);
        }

        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (fs::is_regular_file(entry)) {
                FileInfo fileInfo;
                fileInfo.fileName = entry.path().filename().string();
                fileInfo.filePath = entry.path().string();

                auto ftime = fs::last_write_time(entry);
                auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                    ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now());
                auto mtime = std::chrono::system_clock::to_time_t(sctp);

                std::ostringstream oss;
                oss << std::put_time(std::localtime(&mtime), "%Y-%m-%d %H:%M:%S");
                fileInfo.lastUpdateDate = oss.str();

                fileInfo.lastModifiedTime = mtime;
                files.push_back(fileInfo);
            }
        }

        std::sort(files.begin(), files.end(), [](const FileInfo& a, const FileInfo& b) {
            return a.lastModifiedTime > b.lastModifiedTime;
        });

    } catch (const std::exception& e) {
        std::cerr << "Error accessing folder: " << e.what() << std::endl;
    }

    return files;
}


std::string FileManager::saveAs() {
    std::string projectsPath = Config::getInstance().getProjectsPath();

    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to initialize COM.");
    }

    std::string filePath;

    IFileSaveDialog* pFileSave;
    hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_ALL, IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

    if (SUCCEEDED(hr)) {
        if (!projectsPath.empty()) {
            IShellItem* pFolder;
            std::wstring wProjectsPath = std::wstring(projectsPath.begin(), projectsPath.end());
            hr = SHCreateItemFromParsingName(wProjectsPath.c_str(), nullptr, IID_PPV_ARGS(&pFolder));
            if (SUCCEEDED(hr)) {
                pFileSave->SetFolder(pFolder);
                pFolder->Release();
            }
        }

        COMDLG_FILTERSPEC fileTypes[] = {
            {L"Project Files (*.prj)", L"*.prj"},
        };
        pFileSave->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
        pFileSave->SetDefaultExtension(L"prj");

        hr = pFileSave->Show(nullptr);
        if (SUCCEEDED(hr)) {
            IShellItem* pItem;
            hr = pFileSave->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                if (SUCCEEDED(hr)) {
                    std::wstring wFilePath(pszFilePath);
                    filePath = std::string(wFilePath.begin(), wFilePath.end());
                    CoTaskMemFree(pszFilePath);
                }
                pItem->Release();
            }
        }
        pFileSave->Release();
    }

    CoUninitialize();

    if (filePath.empty()) {
        throw std::runtime_error("No file selected or operation canceled.");
    }

    return filePath;
}

bool FileManager::deleteFile(const std::string& filePath) {
    try {
        std::filesystem::remove(filePath);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error deleting file: " << e.what() << std::endl;
        return false;
    }
}

bool FileManager::createCopy(const std::string& sourceFilePath, const std::string& destinationFolderPath) {
    std::string destinationFilePath = selectFile();
    if (destinationFilePath.empty()) {
        std::cout << "No destination file selected or an error occurred." << std::endl;
        return false;
    }

    try {
        std::filesystem::copy(sourceFilePath, destinationFilePath);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error copying file: " << e.what() << std::endl;
        return false;
    }
}

std::string FileManager::getExportObjPath() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to initialize COM.");
    }

    std::string filePath;

    IFileSaveDialog* pFileSave;
    hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_ALL,
                         IID_IFileSaveDialog, reinterpret_cast<void**>(&pFileSave));

    if (SUCCEEDED(hr)) {
        COMDLG_FILTERSPEC fileTypes[] = {
            {L"OBJ Files (*.obj)", L"*.obj"}
        };
        pFileSave->SetFileTypes(ARRAYSIZE(fileTypes), fileTypes);
        pFileSave->SetDefaultExtension(L"obj");
        pFileSave->SetTitle(L"Export as OBJ");

        hr = pFileSave->Show(nullptr);
        if (SUCCEEDED(hr)) {
            IShellItem* pItem;
            hr = pFileSave->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
                if (SUCCEEDED(hr)) {
                    std::wstring wFilePath(pszFilePath);
                    filePath = std::string(wFilePath.begin(), wFilePath.end());

                    if (filePath.substr(filePath.length() - 4) != ".obj") {
                        filePath += ".obj";
                    }

                    CoTaskMemFree(pszFilePath);
                }
                pItem->Release();
            }
        }
        pFileSave->Release();
    }

    CoUninitialize();

    if (filePath.empty()) {
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["no_file_selected"]);
    }

    return filePath;
}

bool FileManager::exportToObj(const std::string& filePath,
                            const std::vector<std::string>& vertices,
                            const std::vector<std::string>& faces) {
    try {
        ObjFile objFile;
        objFile.setVertices(vertices);
        objFile.setFaces(faces);
        objFile.write(filePath);
        return true;
    } catch (const std::exception& e) {
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["error_exporting"]);
        std::cerr << "Error exporting to OBJ: " << e.what() << std::endl;
        return false;
    }
}
