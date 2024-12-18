#include "ObjectsMenu.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../utils/WindowManager.hpp"
#include "../../core/ui/NumberInput.hpp"
#include "../../core/ui/Input.hpp"
#include "../../core/ui/ScrollView.hpp"
#include "../../core/ui/Expandable.hpp"
#include "../../utils/LanguageManager.hpp"
ObjectsMenu::ObjectsMenu() : currentSelectedObject(nullptr) {}

void ObjectsMenu::createUI() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    objectList.clear();
    components.clear();

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 20),
        20,
        languagePack["Objects"]
    );
    objectList.push_back(title);

    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    std::vector<std::shared_ptr<Object3d>> objects = scene.getObjects();

    auto scrollView = std::make_shared<ScrollView>(
        sf::Vector2f(parentPosition.x + 20, 60),
        sf::Vector2f(310, 300),
        300.f
    );

    for (int i = 1; i < objects.size(); ++i) {
        auto objButton = std::make_shared<Button>(
            sf::Vector2f(parentPosition.x + 20, 60 + 30 * (i - 1)),
            sf::Vector2f(200, 30),
            languagePack["Object"].get<std::string>() + " " + std::to_string(i)
        );

        objButton->setOnClick([this, i, &scene]() {
            scene.onChangeSelectedObjectIndex(i);
            currentSelectedObject = scene.getObjects()[i];
            updatePropertiesMenu();
        });

        scrollView->addComponent(objButton);
    }

    objectList.push_back(scrollView);

    int selectedIndex = scene.getSelectedObjectIndex();
    if (selectedIndex > 0 && selectedIndex < objects.size()) {
        currentSelectedObject = objects[selectedIndex];
        addObjectPropertiesMenu(currentSelectedObject);
    }
}

void ObjectsMenu::draw(sf::RenderWindow &window) {

    if (objectList.empty()) {
        createUI();
    }

    for (const auto& component : objectList) {
        component->draw(window);
    }

    for (const auto& component : objectPropertiesMenu) {
        component->draw(window);
    }
}

void ObjectsMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (objectList.empty()) {
        createUI();
    }

    for (const auto& component : objectList) {
        component->handleEvent(event, window);
    }

    for (const auto& component : objectPropertiesMenu) {
        component->handleEvent(event, window);
    }
}

void ObjectsMenu::onActivate() {
    createUI();
}

void ObjectsMenu::onDeactivate() {
    objectList.clear();
    objectPropertiesMenu.clear();
}

void ObjectsMenu::addObjectPropertiesMenu(std::shared_ptr<Object3d> object) {
    currentSelectedObject = object;
    updatePropertiesMenu();
}

void ObjectsMenu::updatePropertiesMenu() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    objectPropertiesMenu.clear();
    int marginTop = 400;

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 10),
        20,
        languagePack["Properties"]
    );
    objectPropertiesMenu.push_back(title);

///////////////////////////////////////////////////////////////////

    auto scale = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 50),
        20,
        languagePack["Scale"]
    );
    objectPropertiesMenu.push_back(scale);

    auto scaleX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 80),
        sf::Vector2f(90, 50),
        "X"
    );
    scaleX->setValue(currentSelectedObject->getScale().x);

    scaleX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->setScale(Vector3(value - currentSelectedObject->getScale().x, 0.0f, 0.0f));
    }));
    objectPropertiesMenu.push_back(scaleX);

    auto scaleY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 80),
        sf::Vector2f(90, 50),
        "Y"
    );
    scaleY->setValue(currentSelectedObject->getScale().y);
    scaleY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->setScale(Vector3(0.0f, value - currentSelectedObject->getScale().y , 0.0f));
    }));
    objectPropertiesMenu.push_back(scaleY);

    auto scaleZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 80),
        sf::Vector2f(90, 50),
        "Z"
    );
    scaleZ->setValue(currentSelectedObject->getScale().z);
    scaleZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->setScale(Vector3(0.0f, 0.0f, value - currentSelectedObject->getScale().z));
    }));

    objectPropertiesMenu.push_back(scaleZ);

///////////////////////////////////////////////////////////////////

    auto position = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 150),
        20,
        languagePack["Position"]
    );
    objectPropertiesMenu.push_back(position);

    auto positionX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 180),
        sf::Vector2f(90, 50),
        "X"
    );
    positionX->setValue(currentSelectedObject->getPosition().x);

    positionX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->translate(Vector3(value - currentSelectedObject->getPosition().x, 0.0f, 0.0f));
    }));
    objectPropertiesMenu.push_back(positionX);

    auto positionY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 180),
        sf::Vector2f(90, 50),
        "Y"
    );
    positionY->setValue(currentSelectedObject->getPosition().y);
    positionY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->translate(Vector3(0.0f, value - currentSelectedObject->getPosition().y , 0.0f));
    }));
    objectPropertiesMenu.push_back(positionY);

    auto positionZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 180),
        sf::Vector2f(90, 50),
        "Z"
    );
    positionZ->setValue(currentSelectedObject->getPosition().z);
    positionZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->translate(Vector3(0.0f, 0.0f, value - currentSelectedObject->getPosition().z));
    }));

///////////////////////////////////////////////////////////////////

    objectPropertiesMenu.push_back(positionZ);
    auto rotation = std::make_shared<Text>(
     sf::Vector2f(parentPosition.x + 20, marginTop + 250),
             20,
             languagePack["Rotation"]
     );
    objectPropertiesMenu.push_back(rotation);

    auto rotationX = std::make_shared<NumberInput>(
    sf::Vector2f(parentPosition.x + 20, marginTop + 280),
    sf::Vector2f(90, 50),
    "X",
        0.1f
);
    rotationX->setValue(currentSelectedObject->getRotation().x);
    rotationX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        Vector3 currentRotation = currentSelectedObject->getRotation();
        currentSelectedObject->setRotation(Vector3(value - currentRotation.x, currentRotation.y, currentRotation.z));
    }));
    objectPropertiesMenu.push_back(rotationX);

    auto rotationY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 280),
        sf::Vector2f(90, 50),
        "Y",
        0.1f
    );
    rotationY->setValue(currentSelectedObject->getRotation().y);
    rotationY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        Vector3 currentRotation = currentSelectedObject->getRotation();
        currentSelectedObject->setRotation(Vector3(currentRotation.x, value - currentRotation.y, currentRotation.z));
    }));
    objectPropertiesMenu.push_back(rotationY);

    auto rotationZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 280),
        sf::Vector2f(90, 50),
        "Z",
        0.1f
    );
    rotationZ->setValue(currentSelectedObject->getRotation().z);
    rotationZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        Vector3 currentRotation = currentSelectedObject->getRotation();
        currentSelectedObject->setRotation(Vector3(currentRotation.x, currentRotation.y, value - currentRotation.z));
    }));
    objectPropertiesMenu.push_back(rotationZ);
}
