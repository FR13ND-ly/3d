#ifndef FACES_MENU_HPP
#define FACES_MENU_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../../core/ui/Component.hpp"
#include "../../core/ui/Text.hpp"
#include "../../core/ui/Button.hpp"
#include "Menu.hpp"
#include "../../core/objects/Object3d.hpp"

class FacesMenu: public Menu {
public:
    FacesMenu();

    virtual void onActivate();
    virtual void onDeactivate();

    void draw(sf::RenderWindow &window) override;
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window) override;

private:
    void createUI();

    std::vector<std::shared_ptr<Component>> objectList;
    std::vector<std::shared_ptr<Component>> facePropertiesMenu;

    std::shared_ptr<Object3d> currentSelectedObject;


    void addObjectPropertiesMenu(std::shared_ptr<Object3d> object);
    void addFacePropertiesMenu();
};

#endif