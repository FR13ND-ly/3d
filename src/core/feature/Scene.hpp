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
#include "../../utils/ZBuffer.hpp"

class Scene : public Component {
public:
    static Scene& getInstance(sf::RenderWindow& window);

    void addObject(std::shared_ptr<Object3d> object);
    void addObject(const std::string& objectType, float param1 = 1.0f, int param2 = 16);
    const std::vector<std::shared_ptr<Object3d>>& getObjects() const;

    void setCamera(Camera &camera);
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
    bool getFacesEditMode();
    void setFacesEditMode(bool value);
    bool getObjectsEditMode();
    void setObjectsEditMode(bool value);

    void setCameraPosition(Vector3 position);
    void setCameraYawAndPitch(float yaw, float pitch);
    void setOrbitCenter(const Vector3& center);

    void copyObjectByIndex(int index);
    void deleteObjectByIndex(int index);
    void createCombination();

    void resetObjects();
    Renderer renderer;
    std::vector<unsigned int> selectedObjects;

    bool isObjectSelected(unsigned int i) const;
    void toggleObjectSelected(unsigned int i);
private:
    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;
    ZBuffer zBuffer;

    std::vector<std::shared_ptr<Object3d>> objects;
    Camera camera;
    sf::RenderWindow &window;

    int selectedObjectIndex = 0;

    bool verticesEditMode = false;
    bool objectsEditMode = false;
    bool facesEditMode = false;

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
