#ifndef TOML_HPP
#define TOML_HPP

#include <string>
#include "FileType.hpp"
#include <memory>
#include <cpptoml.h>

class TomlFile : public FileType {
public:
    // Default constructor
    TomlFile() = default;

    // Constructor with initial TOML table
    explicit TomlFile(const std::shared_ptr<cpptoml::table>& table);

    // Read TOML file
    void read(const std::string& filename) override;

    // Write TOML file
    void write(const std::string& filename) override;

    // Getter for TOML table
    const std::shared_ptr<cpptoml::table>& getData() const;

    // Setter for TOML table
    void setData(const std::shared_ptr<cpptoml::table>& table);

private:
    std::shared_ptr<cpptoml::table> tomlTable;
};

#endif // TOML_HPP
