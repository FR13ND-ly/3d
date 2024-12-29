#include "ProjectsManager.hpp"
#include <filesystem>
#include <iostream>
#include <stdexcept>

#include "../../utils/LanguageManager.hpp"
#include "../../utils/files/FileManager.hpp"
#include "../../utils/WindowManager.hpp"
#include "../objects/CustomShape.hpp"
#include "../ui/Snackbar.hpp"

namespace fs = std::filesystem;

ProjectsManager& ProjectsManager::getInstance() {
    static ProjectsManager instance;
    return instance;
}

std::vector<FileInfo> ProjectsManager::getProjects() {
    std::vector<FileInfo> projects;
    std::string projectsPath = Config::getInstance().getProjectsPath();

    try {
        if (!fs::exists(projectsPath)) {
            fs::create_directories(projectsPath);
        }

        auto files = FileManager::getInstance().getFolderFiles(projectsPath);
        for (const auto& file : files) {
            if (fs::path(file.fileName).extension() == ".prj") {
                projects.push_back(file);
            }
        }
    } catch (const std::exception& e) {
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["error_accessing_projects_folder"]);
        std::cerr << "Error accessing projects folder: " << e.what() << std::endl;
    }

    return projects;
}

void ProjectsManager::setProject(std::string projectPath) {
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);

    setSelectedProject(projectPath);
    PrjFile project;
    project.read(selectedProjectPath);
    auto projectData = project.projectData;
    scene.setCameraPosition(Vector3({projectData["camera"]["position"][0], projectData["camera"]["position"][1], projectData["camera"]["position"][2]}));
    scene.setCameraYawAndPitch(projectData["camera"]["yaw"], projectData["camera"]["pitch"]);
    scene.setOrbitCenter(Vector3({projectData["camera"]["orbitCenter"][0], projectData["camera"]["orbitCenter"][1], projectData["camera"]["orbitCenter"][2]}));
    scene.resetObjects();
    for (auto object : projectData["objects"]) {
        std::vector<Vector3> vertices;
        for (const auto& vertex : object["vertices"]) {
            vertices.push_back(Vector3(vertex[0], vertex[1], vertex[2]));
        }
        std::vector<std::array<int, 7>> faces;
        for (const auto& face : object["faces"]) {
            faces.push_back({face[0], face[1], face[2], face[3], face[4], face[5], face[6]});
        }
        CustomShape obj = CustomShape(vertices, faces);
        obj.translate(Vector3({object["position"][0], object["position"][1], object["position"][2]}));
        obj.setRotation(Vector3({object["rotation"][0], object["rotation"][1], object["rotation"][2]}));
        obj.setScale(Vector3({static_cast<float>(object["scale"][0])  - 1.f, static_cast<float>(object["scale"][1]) - 1.f, static_cast<float>(object["scale"][2]) - 1.f}));
        scene.addObject(std::make_shared<CustomShape>(obj));
    }
}

void ProjectsManager::createProject() {
    FileManager& fileManager = FileManager::getInstance();
    std::string projectPath = fileManager.saveAs();
    PrjFile prjFile;
    prjFile.write(projectPath);
    setSelectedProject(projectPath);
}

void ProjectsManager::updateProject(const Scene& scene) {
    PrjFile prjFile;
    prjFile.setCamera(
        scene.getCamera().getPosition(),
        scene.getCamera().getYaw(),
        scene.getCamera().getPitch(),
        scene.getCamera().getOrbitCenter()
    );

    int i = 0;
    for (const auto& object : scene.getObjects()) {
        if (i == 0) {
            i++;
            continue;
        }
        nlohmann::json objJson;
        objJson["position"] = {object->getPosition().x, object->getPosition().y, object->getPosition().z};
        objJson["rotation"] = {object->getRotation().x, object->getRotation().y, object->getRotation().z};
        objJson["scale"] = {object->getScale().x, object->getScale().y, object->getScale().z};
        for (const auto& vertex : object->getVerticesForJson()) {
            objJson["vertices"].push_back({ vertex[0], vertex[1], vertex[2] });
        }
        for (const auto& face : object->getFaces()) {
            objJson["faces"].push_back({ face[0], face[1], face[2], face[3], face[4], face[5], face[6] });
        }

        prjFile.addObject(objJson);
    }

    prjFile.write(selectedProjectPath);
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    Snackbar::getInstance().addMessage(languagePack["project_saved"]);
}

void ProjectsManager::deleteProject(const std::string& filePath) {
    if (!fs::exists(filePath)) {
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["file_does_not_exist"]);
        throw std::runtime_error("File does not exist: " + filePath);
    }

    fs::remove(filePath);
}


void ProjectsManager::deleteCurrentProject() {
    if (selectedProjectPath.empty()) {
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["no_project_selected"]);
        throw std::runtime_error("No project is currently selected.");
    }

    deleteProject(selectedProjectPath);

    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    Snackbar::getInstance().addMessage(languagePack["project_deleted"]);
}

std::string ProjectsManager::getProjectFilePath(const std::string& projectName) const {
    return Config::getInstance().getProjectsPath() + "/" + projectName + ".prj";
}

void ProjectsManager::setSelectedProject(const std::string& projectPath) {
    selectedProjectPath = projectPath;
}

nlohmann::json ProjectsManager::sceneToJson(const Scene& scene) {
    nlohmann::json json;

    json["camera"]["position"] = { scene.getCamera().getPosition().x, scene.getCamera().getPosition().y, scene.getCamera().getPosition().z };
    json["camera"]["yaw"] = scene.getCamera().getYaw();
    json["camera"]["pitch"] = scene.getCamera().getPitch();

    for (const auto& object : scene.getObjects()) {
        nlohmann::json objJson;
        objJson["position"] = { object->getPosition().x, object->getPosition().y, object->getPosition().z };
        objJson["rotation"] = { object->getRotation().x, object->getRotation().y, object->getRotation().z };
        objJson["scale"] = { object->getScale().x, object->getScale().y, object->getScale().z };
        for (const auto& vertex : object->getVerticesForJson()) {
            objJson["vertices"].push_back({ vertex[0], vertex[1], vertex[2] });
        }
        for (const auto& face : object->getFaces()) {
            objJson["faces"].push_back({ face[0], face[1], face[2], face[3], face[4], face[5], face[6] });
        }

        json["objects"].push_back(objJson);
    }

    return json;
}

bool ProjectsManager::exportAsObj() {
    if (selectedProjectPath.empty()) {
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["no_project_selected"]);
        return false;
    }

    try {
        PrjFile prjFile;
        prjFile.read(selectedProjectPath);

        auto [vertices, faces] = prepareObjData(prjFile.projectData);

        std::string exportPath = FileManager::getInstance().getExportObjPath();
        if (exportPath.empty()) {
            return false;
        }

        if (FileManager::getInstance().exportToObj(exportPath, vertices, faces)) {
            return true;
        } else {
            auto languagePack = LanguageManager::getInstance().getSelectedPack();
            Snackbar::getInstance().addMessage(languagePack["error_exporting"]);
            return false;
        }
    } catch (const std::exception& e) {
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(
            static_cast<std::string>(languagePack["error_exporting"]) + std::string(": ") + e.what()
        );
        return false;
    }
}

std::pair<std::vector<std::string>, std::vector<std::string>>
ProjectsManager::prepareObjData(const nlohmann::json& projectData) {
    std::vector<std::string> vertices;
    std::vector<std::string> faces;
    int vertexOffset = 1;

    for (const auto& object : projectData["objects"]) {
        const auto& objVertices = object["vertices"];
        for (const auto& vertex : objVertices) {
            const auto& position = object["position"];
            const auto& rotation = object["rotation"];
            const auto& scale = object["scale"];

            // Transform vertex (simplified - you might want to add proper rotation)
            float x = vertex[0].get<float>() * scale[0].get<float>() + position[0].get<float>();
            float y = vertex[1].get<float>() * scale[1].get<float>() + position[1].get<float>();
            float z = vertex[2].get<float>() * scale[2].get<float>() + position[2].get<float>();

            std::ostringstream vertexStr;
            vertexStr << "v " << x << " " << y << " " << z;

            // Add color if available
            if (vertex.size() > 3) {
                vertexStr << " " << vertex[3].get<float>()
                         << " " << vertex[4].get<float>()
                         << " " << vertex[5].get<float>();
            }

            vertices.push_back(vertexStr.str());
        }

        const auto& objFaces = object["faces"];
        for (const auto& face : objFaces) {
            std::ostringstream faceStr;
            faceStr << "f";
            for (const auto& index : face) {
                faceStr << " " << (index.get<int>() + vertexOffset);
            }
            faces.push_back(faceStr.str());
        }

        vertexOffset += objVertices.size();
    }

    return {vertices, faces};
}