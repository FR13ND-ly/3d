#ifndef PROJECTS_VIEW_HPP
#define PROJECTS_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../core/ui/Component.hpp"
#include "../core/ui/Text.hpp"
#include "../core/ui/Button.hpp"
#include "View.hpp"

class ProjectsView: public View {
public:
    ProjectsView();

     void onActivate() override;
     void onDeactivate() override;

private:
    void createUI();
    std::vector<std::shared_ptr<Component>> components;
};

#endif
