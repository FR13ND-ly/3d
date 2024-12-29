#ifndef VERTICES_MENU_HPP
#define VERTICES_MENU_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <queue>
#include <vector>
#include "../../core/ui/Component.hpp"
#include "../../core/ui/Text.hpp"
#include "../../core/ui/Button.hpp"
#include "Menu.hpp"
#include "../../core/objects/Object3d.hpp"
#include "../../core/ui/NumberInput.hpp"
#include "../../core/ui/ScrollView.hpp"

class VerticesMenu: public Menu {
public:
    VerticesMenu();

    virtual void onActivate();
    virtual void onDeactivate();

    void draw(sf::RenderWindow &window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

private:
    void createUI();

    std::vector<std::shared_ptr<Component>> objectList;
    std::vector<std::shared_ptr<Component>> vertexPropertiesMenu;
    std::shared_ptr<ScrollView> objectsScroll;
    std::shared_ptr<Object3d> currentSelectedObject;
    std::queue<std::function<void()>> pendingOperations;

    void addObjectPropertiesMenu(std::shared_ptr<Object3d> object);
    void createVertexPropertiesMenu();
    void updateVertexPropertiesMenu();
    void updateVerticesButtons();

    std::shared_ptr<Text> title;
    std::shared_ptr<Text> position;
    std::shared_ptr<Button> addVertexButton;
    std::shared_ptr<NumberInput> positionX;
    std::shared_ptr<NumberInput> positionY;
    std::shared_ptr<NumberInput> positionZ;
    std::shared_ptr<Button> createEdgeButton;
    std::shared_ptr<Button> createFaceButton;
};

#endif