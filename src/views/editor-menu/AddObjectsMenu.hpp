#ifndef ADD_OBJECTS_MENU_HPP
#define ADD_OBJECTS_MENU_HPP


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
