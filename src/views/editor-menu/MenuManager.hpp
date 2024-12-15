#ifndef MENU_MANAGER_HPP
#define MENU_MANAGER_HPP

#include <map>
#include <memory>
#include <string>
#include "AddObjectsMenu.hpp"
#include "ObjectsMenu.hpp"
#include "../../core/ui/Component.hpp"
#include "Menu.hpp"

class MenuManager : public Component {
public:
    MenuManager();

    static MenuManager& getInstance();

    void addView(const std::string& name, std::shared_ptr<Menu> menu);
    void switchTo(const std::string& name);
    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;
    void draw(sf::RenderWindow& window) override;


    void setParentPosition(const sf::Vector2f& position);
    void setParentSize(const sf::Vector2f& size);

    float getHeight() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

protected:
    sf::Vector2f parentSize = {350.f, 1180.f};
    sf::Vector2f parentPosition = {1920.f - parentSize.x, 0.f};

private:
    void createUI();
    std::map<std::string, std::shared_ptr<Menu>> menus;
    std::shared_ptr<Menu> currentMenu = nullptr;

    MenuManager(const MenuManager&) = delete;
    MenuManager& operator=(const MenuManager&) = delete;

    std::vector<std::shared_ptr<Component>> components;
};

#endif
