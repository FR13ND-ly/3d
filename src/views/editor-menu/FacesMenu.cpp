#include "FacesMenu.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../utils/WindowManager.hpp"
#include "../../core/ui/NumberInput.hpp"
#include "../../core/ui/Input.hpp"
#include "../../core/ui/ScrollView.hpp"
#include "../../core/ui/Expandable.hpp"
#include "../../utils/LanguageManager.hpp"

FacesMenu::FacesMenu() : currentSelectedObject(nullptr) {}

void FacesMenu::createUI() {
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
        auto objButton = std::make_shared<Expandable>(
            sf::Vector2f(parentPosition.x + 20, 60 + 30 * (i - 1)),
            sf::Vector2f(200, 30),
            languagePack["Object"].get<std::string>() + " " + std::to_string(i)
        );

        objButton->setOnClick([this, i, &scene]() {
            scene.onChangeSelectedObjectIndex(i);
            currentSelectedObject = scene.getObjects()[i];
        });

        int j = 0;
        for (auto face : objects[i]->getFaces()) {
            auto faceButton = std::make_shared<Button>(
            sf::Vector2f(parentPosition.x + 40, 60 + 30 * (i - 1)),
            sf::Vector2f(180, 30),
             languagePack["Face"].get<std::string>() + " " + std::to_string(j + 1)
        );

            faceButton->setOnClick([this, j, faceButton]() {
                auto& selectedFaces = currentSelectedObject->selectedFaces;
                auto it = std::find(selectedFaces.begin(), selectedFaces.end(), j);
                if (it != selectedFaces.end()) {
                    selectedFaces.erase(it);
                    faceButton->setColor(sf::Color::White);
                } else {
                    selectedFaces.push_back(j);
                    faceButton->setColor(sf::Color(255, 255, 100));
                }
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
        if (currentSelectedObject->selectedFaces.size() > 0) {
            addFacePropertiesMenu();
        }
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
}

void FacesMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (objectList.empty()) {
        createUI();
    }

    for (const auto& component : objectList) {
        component->handleEvent(event, window);
    }
    for (const auto& component : facePropertiesMenu) {
        component->handleEvent(event, window);
    }
}

void FacesMenu::onActivate() {
    createUI();
    objectList.clear();
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    scene.setFacesEditMode(false);
}

void FacesMenu::onDeactivate() {
    objectList.clear();
    facePropertiesMenu.clear();
    objectList.clear();
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    scene.setFacesEditMode(false);
}


void FacesMenu::addFacePropertiesMenu() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    facePropertiesMenu.clear();
    int marginTop = 400;

    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 10),
        20,
        languagePack["Face properties"]
    );
    facePropertiesMenu.push_back(title);

    auto scaleX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 50),
        sf::Vector2f(90, 50),
        languagePack["Scale"]
    );

    scaleX->setOnClick(static_cast<std::function<void(float)>>([this, scaleX](float value) {
        currentSelectedObject->scaleFaces(value);
        scaleX->setValue(0);
    }));
    facePropertiesMenu.push_back(scaleX);


///////////////////////////////////////////////////////////////////

    auto position = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 120),
        20,
        languagePack["Position"]
    );
    facePropertiesMenu.push_back(position);

    auto positionX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 150),
        sf::Vector2f(90, 50),
        "X"
    );

    positionX->setOnClick(static_cast<std::function<void(float)>>([this, positionX](float value) {
        currentSelectedObject->moveFaces(Vector3(value, 0.0f, 0.0f));
        positionX->setValue(0);
    }));
    facePropertiesMenu.push_back(positionX);

    auto positionY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 150),
        sf::Vector2f(90, 50),
        "Y"
    );
    positionY->setOnClick(static_cast<std::function<void(float)>>([this, positionY](float value) {
        currentSelectedObject->moveFaces(Vector3(0.0f, value , 0.0f));
        positionY->setValue(0);
    }));
    facePropertiesMenu.push_back(positionY);

    auto positionZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 150),
        sf::Vector2f(90, 50),
        "Z"
    );
    positionZ->setOnClick(static_cast<std::function<void(float)>>([this, positionZ](float value) {
        currentSelectedObject->moveFaces(Vector3(0.0f, 0.0f, value));
        positionZ->setValue(0);
    }));

///////////////////////////////////////////////////////////////////

    facePropertiesMenu.push_back(positionZ);
    auto rotation = std::make_shared<Text>(
     sf::Vector2f(parentPosition.x + 20, marginTop + 220),
             20,
             languagePack["Rotation"]
     );
    facePropertiesMenu.push_back(rotation);

    auto rotationX = std::make_shared<NumberInput>(
    sf::Vector2f(parentPosition.x + 20, marginTop + 250),
    sf::Vector2f(90, 50),
    "X",
        0.1f
    );
    rotationX->setOnClick(static_cast<std::function<void(float)>>([this, rotationX](float value) {
        currentSelectedObject->rotateFaces(value, 'x');
        rotationX->setValue(0);
    }));
    facePropertiesMenu.push_back(rotationX);

    auto rotationY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 250),
        sf::Vector2f(90, 50),
        "Y",
        0.1f
    );
    rotationY->setOnClick(static_cast<std::function<void(float)>>([this, rotationY](float value) {
        currentSelectedObject->rotateFaces(value, 'y');
        rotationY->setValue(0);
    }));
    facePropertiesMenu.push_back(rotationY);

    auto rotationZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 250),
        sf::Vector2f(90, 50),
        "Z",
        0.1f
    );
    rotationZ->setOnClick(static_cast<std::function<void(float)>>([this, rotationZ](float value) {
        currentSelectedObject->rotateFaces(value, 'z');
        rotationZ->setValue(0);
    }));
    facePropertiesMenu.push_back(rotationZ);
}
