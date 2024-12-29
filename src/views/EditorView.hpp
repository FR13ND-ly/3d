#ifndef EDITOR_VIEW_HPP
#define EDITOR_VIEW_HPP

#include <memory>
#include "View.hpp"
#include "../core/feature/Scene.hpp"
#include "editor-menu/MenuManager.hpp"

class EditorView : public View {
public:
    EditorView();
    void clearComponents();
    void onActivate() override;
    void onDeactivate() override;
    void handleEvent(const sf::Event &event, sf::RenderWindow &window) override;
private:
    std::shared_ptr<Scene> scene;
    std::shared_ptr<MenuManager> editorMenu;
    void createUI();
};

#endif
