#include "ViewsManager.hpp"
#include "viewSwitcher.hpp"
#include <iostream>

void switchToView(const std::string& name) {
    ViewsManager::getInstance().switchTo(name);
}