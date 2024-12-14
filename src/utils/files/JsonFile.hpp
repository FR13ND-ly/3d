#ifndef JSON_HPP
#define JSON_HPP

#include <string>
#include "FileType.hpp"
#include <nlohmann/json.hpp>


class JsonFile : public FileType {
public:
    JsonFile() = default;

    explicit JsonFile(const nlohmann::json& data);

    void read(const std::string& filename) override;
    void write(const std::string& filename) override;

    const nlohmann::json& getData() const;
    void setData(const nlohmann::json& data);

private:
    nlohmann::json jsonData;
};

#endif // JSON_HPP
