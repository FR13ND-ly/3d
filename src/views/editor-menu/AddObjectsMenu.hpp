#ifndef ADD_OBJECTS_MENU_HPP
#define ADD_OBJECTS_MENU_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../../core/ui/Component.hpp"
#include "../../core/ui/Text.hpp"
#include "../../core/ui/Button.hpp"
#include "Menu.hpp"

class AddObjectsMenu: public Menu {
public:
    AddObjectsMenu();

    virtual void onActivate();
    virtual void onDeactivate();

private:
    void createUI();
};

#endif
