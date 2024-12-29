#ifndef PROJECT_SETTINGS_HPP
#define PROJECT_SETTINGS_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include <vector>
#include "../../core/ui/Component.hpp"
#include "../../core/ui/Text.hpp"
#include "../../core/ui/Button.hpp"
#include "Menu.hpp"

class ProjectSettings: public Menu {
public:
    ProjectSettings();

    virtual void onActivate();
    virtual void onDeactivate();

private:
    void createUI();
    std::queue<std::function<void()>> pendingOperations;
};

#endif
