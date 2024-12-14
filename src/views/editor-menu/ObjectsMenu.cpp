#include "ObjectsMenu.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../utils/WindowManager.hpp"
#include "../../core/ui/NumberInput.hpp"

ObjectsMenu::ObjectsMenu() : currentSelectedObject(nullptr) {
}

void ObjectsMenu::createUI() {
    objectListComponents.clear();

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 20),
        20,
        "Objects"
    );

    objectListComponents.push_back(title);
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    std::vector<std::shared_ptr<Object3d>> objects = scene.getObjects();

    for (int i = 1; i < objects.size(); i++) {
        auto obj = std::make_shared<Button>(
            sf::Vector2f(parentPosition.x + 20, 20 + 32 * i),
            sf::Vector2f(200, 30),
            "Object " + std::to_string(i)
        );

        obj->setOnClick([this, i, &scene]() {
            scene.onChangeSelectedObjectIndex(i);
            currentSelectedObject = scene.getObjects()[i];
            updatePropertiesMenu();
        });
        objectListComponents.push_back(obj);
    }

    if (scene.getSelectedObjectIndex() > 0 && scene.getSelectedObjectIndex() < objects.size()) {
        currentSelectedObject = objects[scene.getSelectedObjectIndex()];
        addObjectPropertiesMenu(currentSelectedObject, 50 + 32 * scene.getObjects().size());
    }
}

void ObjectsMenu::updatePropertiesMenu() {
    propertiesMenuComponents.clear();

    if (!currentSelectedObject) return;


    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    int marginTop = 50 + 32 * scene.getObjects().size();

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 10),
        20,
        "Properties"
    );
    propertiesMenuComponents.push_back(title);

///////////////////////////////////////////////////////////////////

    auto scale = std::make_shared<Text>(
            sf::Vector2f(parentPosition.x + 20, marginTop + 50),
            20,
            "Scale: "
        );
    propertiesMenuComponents.push_back(scale);

    auto scaleX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 80),
        sf::Vector2f(100, 50),
        "X"
    );
    scaleX->setValue(currentSelectedObject->getScale().x);

    scaleX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->setScale(Vector3(value - currentSelectedObject->getScale().x, 0.0f, 0.0f));
    }));
    propertiesMenuComponents.push_back(scaleX);

    auto scaleY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 80),
        sf::Vector2f(100, 50),
        "Y"
    );
    scaleY->setValue(currentSelectedObject->getScale().y);
    scaleY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->setScale(Vector3(0.0f, value - currentSelectedObject->getScale().y , 0.0f));
    }));
    propertiesMenuComponents.push_back(scaleY);

    auto scaleZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 80),
        sf::Vector2f(100, 50),
        "Z"
    );
    scaleZ->setValue(currentSelectedObject->getScale().z);
    scaleZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->setScale(Vector3(0.0f, 0.0f, value - currentSelectedObject->getScale().z));
    }));

    propertiesMenuComponents.push_back(scaleZ);

///////////////////////////////////////////////////////////////////

    auto position = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 150),
        20,
        "Position: "
    );
    propertiesMenuComponents.push_back(position);

    auto positionX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 180),
        sf::Vector2f(100, 50),
        "X"
    );
    positionX->setValue(currentSelectedObject->getPosition().x);

    positionX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->translate(Vector3(value - currentSelectedObject->getPosition().x, 0.0f, 0.0f));
    }));
    propertiesMenuComponents.push_back(positionX);

    auto positionY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 180),
        sf::Vector2f(100, 50),
        "Y"
    );
    positionY->setValue(currentSelectedObject->getPosition().y);
    positionY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->translate(Vector3(0.0f, value - currentSelectedObject->getPosition().y , 0.0f));
    }));
    propertiesMenuComponents.push_back(positionY);

    auto positionZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 180),
        sf::Vector2f(100, 50),
        "Z"
    );
    positionZ->setValue(currentSelectedObject->getPosition().z);
    positionZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->translate(Vector3(0.0f, 0.0f, value - currentSelectedObject->getPosition().z));
    }));

///////////////////////////////////////////////////////////////////

    propertiesMenuComponents.push_back(positionZ);
    auto rotation = std::make_shared<Text>(
     sf::Vector2f(parentPosition.x + 20, marginTop + 250),
             20,
             "Rotation: "
     );
    propertiesMenuComponents.push_back(rotation);

    auto rotationX = std::make_shared<NumberInput>(
    sf::Vector2f(parentPosition.x + 20, marginTop + 280),
    sf::Vector2f(100, 50),
    "X",
        0.1f
);
    rotationX->setValue(currentSelectedObject->getRotation().x);
    rotationX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        Vector3 currentRotation = currentSelectedObject->getRotation();
        currentSelectedObject->setRotation(Vector3(value - currentRotation.x, currentRotation.y, currentRotation.z));
    }));
    propertiesMenuComponents.push_back(rotationX);

    auto rotationY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 280),
        sf::Vector2f(100, 50),
        "Y",
        0.1f
    );
    rotationY->setValue(currentSelectedObject->getRotation().y);
    rotationY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        Vector3 currentRotation = currentSelectedObject->getRotation();
        currentSelectedObject->setRotation(Vector3(currentRotation.x, value - currentRotation.y, currentRotation.z));
    }));
    propertiesMenuComponents.push_back(rotationY);

    auto rotationZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 280),
        sf::Vector2f(100, 50),
        "Z",
        0.1f
    );
    rotationZ->setValue(currentSelectedObject->getRotation().z);
    rotationZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        Vector3 currentRotation = currentSelectedObject->getRotation();
        currentSelectedObject->setRotation(Vector3(currentRotation.x, currentRotation.y, value - currentRotation.z));
    }));
    propertiesMenuComponents.push_back(rotationZ);
}

void ObjectsMenu::draw(sf::RenderWindow &window) {
    if (objectListComponents.empty()) {
        createUI();
    }

    for (const auto& component : objectListComponents) {
        component->draw(window);
    }

    for (const auto& component : propertiesMenuComponents) {
        component->draw(window);
    }
}

void ObjectsMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (objectListComponents.empty()) {
        createUI();
    }

    for (const auto& component : objectListComponents) {
        component->handleEvent(event, window);
    }

    for (const auto& component : propertiesMenuComponents) {
        component->handleEvent(event, window);
    }
}

void ObjectsMenu::onActivate() {
    createUI();
}

void ObjectsMenu::onDeactivate() {
    objectListComponents.clear();
    propertiesMenuComponents.clear();
}

void ObjectsMenu::addObjectPropertiesMenu(std::shared_ptr<Object3d> object, int marginTop) {
    currentSelectedObject = object;
    updatePropertiesMenu();
}