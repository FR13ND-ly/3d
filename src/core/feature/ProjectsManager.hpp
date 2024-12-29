#ifndef PROJECT_MANAGER_HPP
#define PROJECT_MANAGER_HPP

#include <string>
#include <vector>
#include "../../utils/files/Config.hpp"
#include "../../utils/files/FileManager.hpp"
#include "../../utils/files/PrjFile.hpp"
#include "Scene.hpp"

class ProjectsManager {
public:
    static ProjectsManager& getInstance();

    ProjectsManager(const ProjectsManager&) = delete;
    ProjectsManager& operator=(const ProjectsManager&) = delete;

    std::vector<FileInfo> getProjects();
    void createProject();
    void updateProject(const Scene& scene);
    void deleteProject(const std::string& projectName);
    void deleteCurrentProject();
    void setProject(std::string projectName);
    bool exportAsObj();

private:
    ProjectsManager() = default;
    ~ProjectsManager() = default;

    std::string selectedProjectPath;
    void setSelectedProject(const std::string& projectPath);

    nlohmann::json sceneToJson(const Scene& scene);
    std::string getProjectFilePath(const std::string& projectName) const;

    std::pair<std::vector<std::string>, std::vector<std::string>>
    prepareObjData(const nlohmann::json& projectData);
};

#endif