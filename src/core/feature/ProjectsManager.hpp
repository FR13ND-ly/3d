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

    std::vector<FileInfo> getProjects();
    void setProject(std::string projectPath);
    void createProject();
    void updateProject(const Scene& scene);
    void deleteProject(const std::string& filePath);
    void deleteCurrentProject();
    std::string getProjectFilePath(const std::string& projectName) const;
    bool exportAsObj();

private:
    ProjectsManager() = default;
    ProjectsManager(const ProjectsManager&) = delete;
    ProjectsManager& operator=(const ProjectsManager&) = delete;

    void setSelectedProject(const std::string& projectPath);
    nlohmann::json sceneToJson(const Scene& scene);
    std::pair<std::vector<std::string>, std::vector<std::string>> prepareObjData(const Scene& scene);

    std::string selectedProjectPath;
};


#endif