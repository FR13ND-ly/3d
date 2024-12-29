#ifndef PRJ_FILE_HPP
#define PRJ_FILE_HPP

#include "JsonFile.hpp"
#include <nlohmann/json.hpp>
#include "../math/Vector3.hpp"

class PrjFile : public FileType {
public:
    PrjFile() = default;

    void read(const std::string& filename) override;

    void write(const std::string& filename) override;

    void setCamera(Vector3 position, float yaw, float pitch, Vector3 orbitCenter);
    void addObject(const nlohmann::json& object);

    const nlohmann::json& getCamera() const;
    const std::vector<nlohmann::json>& getObjects() const;

    nlohmann::json projectData = {
        {"camera", {{"position", {-1.5f, 2.0f, -15.0f}}, {"yaw", -2.0f}, {"pitch", -5.0f}, {"orbitCenter", {0.0f, 0.0f, 0.0f}}}},
        {"objects", nlohmann::json::array()}
    };
};

#endif