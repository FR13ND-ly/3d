#ifndef TOML_HPP
#define TOML_HPP

#include <string>
#include "FileType.hpp"
#include <memory>
#include <cpptoml.h>

class TomlFile : public FileType {
public:
    TomlFile() = default;

    explicit TomlFile(const std::shared_ptr<cpptoml::table>& table);

    void read(const std::string& filename) override;

    void write(const std::string& filename) override;

    const std::shared_ptr<cpptoml::table>& getData() const;

    void setData(const std::shared_ptr<cpptoml::table>& table);

private:
    std::shared_ptr<cpptoml::table> tomlTable;
};

#endif