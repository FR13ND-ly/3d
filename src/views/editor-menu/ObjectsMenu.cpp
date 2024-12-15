#include "ObjectsMenu.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../utils/WindowManager.hpp"
#include "../../core/ui/NumberInput.hpp"
#include "../../core/ui/Input.hpp"
#include "../../core/ui/ScrollView.hpp"
#include "../../core/ui/Expandable.hpp"

ObjectsMenu::ObjectsMenu() : currentSelectedObject(nullptr) {}

void ObjectsMenu::createUI() {
    objectList.clear();
    components.clear();

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 20),
        20,
        "Objects"
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
        auto objButton = std::make_shared<Expandable>(
            sf::Vector2f(parentPosition.x + 20, 60 + 30 * (i - 1)),
            sf::Vector2f(200, 30),
            "Object " + std::to_string(i)
        );

        objButton->setOnClick([this, i, &scene]() {
            scene.onChangeSelectedObjectIndex(i);
            currentSelectedObject = scene.getObjects()[i];
            updatePropertiesMenu();
            selectedFaceIndex = -1;
        });

        int j = 0;
        for (auto face : objects[i]->getFaces()) {
            auto faceButton = std::make_shared<Button>(
            sf::Vector2f(parentPosition.x + 40, 60 + 30 * (i - 1)),
            sf::Vector2f(180, 30),
            "Face " + std::to_string(j + 1)
        );

            faceButton->setOnClick([this, j]() {
                selectedFaceIndex = j;
                addFacePropertiesMenu();
            });
            objButton->addContent(faceButton);
            j++;
        }

        scrollView->addComponent(objButton);
    }

    objectList.push_back(scrollView);

    int selectedIndex = scene.getSelectedObjectIndex();
    if (selectedIndex > 0 && selectedIndex < objects.size()) {
        currentSelectedObject = objects[selectedIndex];
        if (selectedFaceIndex >= 0) {
            addFacePropertiesMenu();
        }
        else {
            addObjectPropertiesMenu(currentSelectedObject);
        }
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

    for (const auto& component : facePropertiesMenu) {
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
    for (const auto& component : facePropertiesMenu) {
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
    objectPropertiesMenu.clear();
    facePropertiesMenu.clear();
    int marginTop = 400;

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 10),
        20,
        "Properties"
    );
    objectPropertiesMenu.push_back(title);

///////////////////////////////////////////////////////////////////

    auto scale = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 50),
        20,
        "Scale: "
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
        "Position: "
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
             "Rotation: "
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


void ObjectsMenu::addFacePropertiesMenu() {
    objectPropertiesMenu.clear();
    int marginTop = 400;

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 10),
        20,
        "Face Properties"
    );
    facePropertiesMenu.push_back(title);

///////////////////////////////////////////////////////////////////

    auto verticeOne = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 50),
        20,
        "Vertices 1"
    );
    objectPropertiesMenu.push_back(verticeOne);


    auto verticeOneX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 80),
        sf::Vector2f(90, 50),
        "X"
    );
    verticeOneX->setValue(currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[0].x);

    verticeOneX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->updateFaceVertex(selectedFaceIndex, 0, {value, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[0].y, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[0].z});
    }));
    objectPropertiesMenu.push_back(verticeOneX);

    auto verticeOneY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 80),
        sf::Vector2f(90, 50),
        "Y"
    );
    verticeOneY->setValue(currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[0].y);

    verticeOneY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->updateFaceVertex(selectedFaceIndex, 0, {currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[0].x, value, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[0].z});
    }));
    objectPropertiesMenu.push_back(verticeOneY);

    auto verticeOneZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 80),
        sf::Vector2f(90, 50),
        "Z"
    );
    verticeOneZ->setValue(currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[0].z);

    verticeOneZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->updateFaceVertex(selectedFaceIndex, 0, {currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[0].x, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[0].y, value});
    }));
    objectPropertiesMenu.push_back(verticeOneZ);

///////////////////////////////////////////////////////////////////

    auto verticeTwo = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 150),
        20,
        "Vertices 2"
    );
    objectPropertiesMenu.push_back(verticeTwo);

    auto verticeTwoX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 180),
        sf::Vector2f(90, 50),
        "X"
    );
    verticeTwoX->setValue(currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[1].x);

    verticeTwoX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->updateFaceVertex(selectedFaceIndex, 1, {value, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[1].y, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[1].z});
    }));
    objectPropertiesMenu.push_back(verticeTwoX);

    auto verticeTwoY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 180),
        sf::Vector2f(90, 50),
        "Y"
    );
    verticeTwoY->setValue(currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[1].y);

    verticeTwoY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->updateFaceVertex(selectedFaceIndex, 1, {currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[1].x, value, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[1].z});
    }));
    objectPropertiesMenu.push_back(verticeTwoY);

    auto verticeTwoZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 180),
        sf::Vector2f(90, 50),
        "Z"
    );
    verticeTwoZ->setValue(currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[1].z);

    verticeTwoZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->updateFaceVertex(selectedFaceIndex, 1, {currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[1].x, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[1].y, value});
    }));
    objectPropertiesMenu.push_back(verticeTwoZ);

///////////////////////////////////////////////////////////////////

    auto verticeThree = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 250),
        20,
        "Vertices 3"
    );
    objectPropertiesMenu.push_back(verticeThree);

    auto verticeThreeX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 280),
        sf::Vector2f(90, 50),
        "X"
    );
    verticeThreeX->setValue(currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[2].x);

    verticeThreeX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->updateFaceVertex(selectedFaceIndex, 2, {value, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[2].y, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[2].z});
    }));
    objectPropertiesMenu.push_back(verticeThreeX);

    auto verticeThreeY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 280),
        sf::Vector2f(90, 50),
        "Y"
    );
    verticeThreeY->setValue(currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[2].y);

    verticeThreeY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->updateFaceVertex(selectedFaceIndex, 2, {currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[2].x, value, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[2].z});
    }));
    objectPropertiesMenu.push_back(verticeThreeY);

    auto verticeThreeZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 280),
        sf::Vector2f(90, 50),
        "Z"
    );
    verticeThreeZ->setValue(currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[2].z);

    verticeThreeZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->updateFaceVertex(selectedFaceIndex, 2, {currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[2].x, currentSelectedObject->getFaceVerticesForEditing(selectedFaceIndex)[2].y, value});
    }));
    objectPropertiesMenu.push_back(verticeThreeZ);

////////////////////////////////////////////////////////////////
    auto color = std::make_shared<Input>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 400),
        sf::Vector2f(200, 50),
        "Color"
    );

    color->setValue(currentSelectedObject->getFaceColor(selectedFaceIndex));

    color->setOnClick(std::function<void(std::string)>([this](std::string value) {
        currentSelectedObject->setFaceColor(selectedFaceIndex, value);
    }));
    objectPropertiesMenu.push_back(color);
}
