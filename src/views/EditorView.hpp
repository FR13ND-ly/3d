#ifndef EDITOR_VIEW_HPP
#define EDITOR_VIEW_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include "View.hpp"
#include "../core/ui/Component.hpp"
#include "../core/ui/Button.hpp"
#include "../core/ui/Text.hpp"
#include "../core/feature/Scene.hpp"
#include "../utils/WindowManager.hpp"
#include "../core/objects/Cube.hpp"
#include "../core/objects/Sphere.hpp"

class EditorView : public View {
public:
    EditorView();
    void clearComponents();

private:
    std::shared_ptr<Scene> scene;

    void initializeScene();
    void createUI();
};

#endif
