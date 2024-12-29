#include "FacesMenu.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../core/ui/Container.hpp"
#include "../../utils/WindowManager.hpp"
#include "../../core/ui/NumberInput.hpp"
#include "../../core/ui/Input.hpp"
#include "../../core/ui/ScrollView.hpp"
#include "../../core/ui/Expandable.hpp"
#include "../../core/ui/Snackbar.hpp"
#include "../../utils/LanguageManager.hpp"

FacesMenu::FacesMenu() : currentSelectedObject(nullptr) {}

void FacesMenu::createUI() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    objectList.clear();
    components.clear();

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 20),
        20,
        languagePack["Faces"]
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
            sf::Vector2f(290, 30),
            languagePack["Object"].get<std::string>() + " " + std::to_string(i)
        );

        objButton->setOnClick([this, i, &scene]() {
            scene.onChangeSelectedObjectIndex(i);
            currentSelectedObject = scene.getObjects()[i];
        });

        int j = 0;
        for (auto face : objects[i]->getFaces()) {
            auto faceContainer = std::make_shared<Container>(
                sf::Vector2f(parentPosition.x + 40, 60 + 30 * (j + i)),
                sf::Vector2f(250, 30)
            );
            auto faceButton = std::make_shared<Button>(
                sf::Vector2f(parentPosition.x + 40, 60 + 30 * (j + i)),
                sf::Vector2f(240, 30),
                 languagePack["Face"].get<std::string>() + " " + std::to_string(j + 1)
            );
            if (objects[i]->isFaceHovered(j)) {
                faceButton->setColor(sf::Color(255, 255, 150));
            }
            if (objects[i]->isFaceSelected(j)) {
                faceButton->setColor(sf::Color(255, 255, 100));
            }

            faceButton->setOnClick([this, j, faceButton]() {
                auto& selectedFaces = currentSelectedObject->selectedFaces;
                auto it = std::find(selectedFaces.begin(), selectedFaces.end(), j);
                if (it != selectedFaces.end()) {
                    selectedFaces.erase(it);
                } else {
                    selectedFaces.push_back(j);
                }
                updateFacePropertiesMenu();
            });
            faceButton->setOnHover([this, j]() {
                auto& hoveredFaces = currentSelectedObject->hoveredFaces;
                auto it = std::find(hoveredFaces.begin(), hoveredFaces.end(), j);
                if (it == hoveredFaces.end()) {
                    hoveredFaces.push_back(j);
                }
            });
            faceButton->setOnHoverOut([this, j]() {
                auto& hoveredFaces = currentSelectedObject->hoveredFaces;
                auto it = std::find(hoveredFaces.begin(), hoveredFaces.end(), j);
                if (it != hoveredFaces.end()) {
                    hoveredFaces.erase(it);
                }
            });
            auto deleteFaceButton = std::make_shared<Button>(
                sf::Vector2f(parentPosition.x + 280, 60 + 30 * (j + i)),
                sf::Vector2f(30, 30),
                ""
            );
            deleteFaceButton->setIcon("delete");
            deleteFaceButton->setOnClick([this, j]() {
                pendingOperations.push([this, j]() {
                    currentSelectedObject->deleteFaceByIndex(j);
                    createUI();
                    auto languagePack = LanguageManager::getInstance().getSelectedPack();
                    Snackbar::getInstance().addMessage(languagePack["face_deleted"]);
                });
            });
            faceContainer->addComponent(faceButton);
            faceContainer->addComponent(deleteFaceButton);
            objButton->addContent(faceContainer);
            j++;
        }

        scrollView->addComponent(objButton);
    }

    objectList.push_back(scrollView);
    objectsScroll = scrollView;


    int selectedIndex = scene.getSelectedObjectIndex();
    if (selectedIndex > 1 && selectedIndex < objects.size()) {
        currentSelectedObject = objects[selectedIndex];
    }
}

void FacesMenu::draw(sf::RenderWindow &window) {
    if (objectList.empty()) {
        createUI();
    }
    for (const auto& component : objectList) {
        component->draw(window);
    }

    for (const auto& component : facePropertiesMenu) {
        component->draw(window);
    }
    while (!pendingOperations.empty()) {
        pendingOperations.front()();
        pendingOperations.pop();
    }
}

void FacesMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (objectList.empty()) {
        createUI();
    }
    updateFacesButtons();
    for (const auto& component : objectList) {
        component->handleEvent(event, window);
    }
    for (const auto& component : facePropertiesMenu) {
        component->handleEvent(event, window);
    }
    while (!pendingOperations.empty()) {
        pendingOperations.front()();
        pendingOperations.pop();
    }
}

void FacesMenu::updateFacesButtons() {
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    std::vector<std::shared_ptr<Object3d>> objects = scene.getObjects();
    int selectedIndex = scene.getSelectedObjectIndex();
    if (selectedIndex > 0 && selectedIndex < objects.size()) {
        if (currentSelectedObject && objects[selectedIndex] != currentSelectedObject) {
            currentSelectedObject->selectedFaces.clear();
        }
        currentSelectedObject = objects[selectedIndex];
    }
    auto objectsList = objectsScroll->getComponents();
    for (int i = 0; i < objectsList.size(); ++i) {
        auto objButton = std::dynamic_pointer_cast<Expandable>(objectsList[i]);
        if (currentSelectedObject) {
            if (objects[i + 1] == currentSelectedObject) {
                objButton->setTextColor(sf::Color::Yellow);
            }
            else {
                objButton->setTextColor(sf::Color::Black);
            }
            objButton->setExpanded(objects[i + 1] == currentSelectedObject);
        }
        if (objButton) {
            auto content= objButton->getContent();
            int j = 0;
            for (const auto& faceComponent: content) {
                auto faceContainer = std::dynamic_pointer_cast<Container>(faceComponent);
                auto faceButton = std::dynamic_pointer_cast<Button>(faceContainer->getComponents()[0]);
                if (faceButton) {
                    if (objects[i + 1]->isFaceHovered(j)) {
                        faceButton->setColor(sf::Color(255, 255, 150));
                    }
                    if (objects[i + 1]->isFaceSelected(j)) {
                        faceButton->setColor(sf::Color(255, 255, 100));
                    }
                    if (!objects[i + 1]->isFaceHovered(j) && !objects[i + 1]->isFaceSelected(j)) {
                        faceButton->setColor(sf::Color::White);
                    }
                }
                j++;
            }
        }
    }
    updateFacePropertiesMenu();
}


void FacesMenu::onActivate() {
    createUI();
    objectList.clear();
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    scene.setFacesEditMode(true);
    createFacePropertiesMenu();
}

void FacesMenu::onDeactivate() {
    objectList.clear();
    facePropertiesMenu.clear();
    objectList.clear();
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    scene.setFacesEditMode(false);
}

void FacesMenu::createFacePropertiesMenu() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 410),
        20,
        languagePack["Face properties"]
    );
    title->setBold();
    scaleX = std::make_shared<NumberInput>(
       sf::Vector2f(parentPosition.x + 20, 450),
       sf::Vector2f(90, 50),
       languagePack["Scale"]
   );
    scaleX->setValue(0);
    scaleX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->scaleFaces(value);
        scaleX->setValue(0.00f);
    }));
    position = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 520),
        20,
        languagePack["Position"]
    );
    positionX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, 550),
        sf::Vector2f(90, 50),
        "X"
    );
    positionX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->moveFaces(Vector3(value, 0.0f, 0.0f));
        positionX->setValue(0.00f);
    }));
    positionY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, 550),
        sf::Vector2f(90, 50),
        "X"
    );
    positionY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->moveFaces(Vector3(0.0f, value , 0.0f));
        positionY->setValue(0.00f);
    }));
    positionZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, 550),
        sf::Vector2f(90, 50),
        "X"
    );
    positionZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->moveFaces(Vector3(0.0f, 0.0f, value));
        positionZ->setValue(0.00f);
    }));
    rotation = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 620),
        20,
        languagePack["Rotation"]
    );
    rotationX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, 650),
        sf::Vector2f(90, 50),
        "X",
        0.1f
    );
    rotationX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->rotateFaces(value, 'x');
        rotationX->setValue(0.00f);
    }));
    rotationY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, 650),
        sf::Vector2f(90, 50),
        "Y",
        0.1f
    );
    rotationY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        currentSelectedObject->rotateFaces(value, 'y');
        rotationY->setValue(0.00f);
    }));
    rotationZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, 650),
        sf::Vector2f(90, 50),
        "Z",
        0.1f
    );
    rotationZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
       currentSelectedObject->rotateFaces(value, 'z');
       rotationZ->setValue(0.00f);
    }));
    color = std::make_shared<Input>(
       sf::Vector2f(parentPosition.x + 20, 750),
       sf::Vector2f(200, 50),
       languagePack["Color"],
       languagePack["Color"]
    );
    color->setOnClick(std::function([this](std::string value) {
        if (currentSelectedObject->selectedFaces.size() == 1) {
            currentSelectedObject->setFaceColor(currentSelectedObject->selectedFaces[0], value);
        }
        else {
            for (int i = 0; i < currentSelectedObject->selectedFaces.size(); i++) {
                currentSelectedObject->setFaceColor(currentSelectedObject->selectedFaces[i], value);
            }
        }
    }));
    changeNormal = std::make_shared<Button>(
      sf::Vector2f(parentPosition.x + 20, 820),
      sf::Vector2f(200, 50),
      languagePack["inverse_normal"]
    );
    changeNormal->setIcon("change_face_normal");
    changeNormal->setOnClick(([this]() {
        currentSelectedObject->inverseFaceNormals();
    }));
}

void FacesMenu::updateFacePropertiesMenu() {
    facePropertiesMenu.clear();
    if (!currentSelectedObject) return;
    if (currentSelectedObject->selectedFaces.size() == 0) return;

    facePropertiesMenu.push_back(title);
    facePropertiesMenu.push_back(scaleX);
    facePropertiesMenu.push_back(position);
    facePropertiesMenu.push_back(positionX);
    facePropertiesMenu.push_back(positionY);
    facePropertiesMenu.push_back(positionZ);
    facePropertiesMenu.push_back(rotation);
    facePropertiesMenu.push_back(rotationX);
    facePropertiesMenu.push_back(rotationY);
    facePropertiesMenu.push_back(rotationZ);
    if (currentSelectedObject->selectedFaces.size() == 1) {
        std::string faceColor = currentSelectedObject->getFaceColor(currentSelectedObject->selectedFaces[0]);
        if (color->lastUpdatedValue != faceColor) {
            color->setValue(faceColor);
        }
    }
    else {
        if (color->lastUpdatedValue != "") {
            color->setValue("");
        }
    }
    facePropertiesMenu.push_back(color);
    facePropertiesMenu.push_back(changeNormal);
}
