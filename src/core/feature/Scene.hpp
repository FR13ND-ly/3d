#ifndef SCENE_HPP
#define SCENE_HPP

#include "../objects/Object3d.hpp"
#include "../objects/GridPlane.hpp"

#include "../ui/Component.hpp"
#include "Camera.hpp"
#include "Renderer.hpp"
#include <vector>
#include <memory>
#include "../../utils/files/Config.hpp"

class Scene : public Component {
public:
    static Scene& getInstance(sf::RenderWindow& window);

    void addObject(std::shared_ptr<Object3d> object);
    void addObject(const std::string& objectType, float param1 = 1.0f, int param2 = 16);
    const std::vector<std::shared_ptr<Object3d>>& getObjects() const;

    void setCamera(const Camera &camera);
    const Camera& getCamera() const;
    Camera& getCamera();

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window) override;
    void draw(sf::RenderWindow &window) override;

    void onChangeSelectedObjectIndex(int selectedObjectIndex);
    int getSelectedObjectIndex() const;
    explicit Scene(sf::RenderWindow &window);

    float getHeight() const override;

    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

    bool getVerticesEditMode();
    void setVerticesEditMode(bool value);

private:
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    std::vector<std::shared_ptr<Object3d>> objects;
    Camera camera;
    Renderer renderer;
    sf::RenderWindow &window;
    int selectedObjectIndex = 0;
    bool verticesEditMode = false;

    float yawVelocity;
    float pitchVelocity;
    float dampingFactor;
    sf::Clock clock;

    void handleScroll(sf::Event::MouseWheelScrollEvent event);
    void handleKeyPressed(sf::Keyboard::Key key);
    void handleMouseMoved();
    void update(float deltaTime);

    bool inBounds(const sf::Vector2i &mousePos) const override;
};

#endif
