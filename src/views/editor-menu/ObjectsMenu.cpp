#include "ObjectsMenu.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../core/objects/ObjectsFactory.hpp"
#include "../../core/ui/Container.hpp"
#include "../../utils/WindowManager.hpp"
#include "../../core/ui/NumberInput.hpp"
#include "../../core/ui/Snackbar.hpp"
#include "../../utils/LanguageManager.hpp"

ObjectsMenu::ObjectsMenu() : currentSelectedObject(nullptr) {
}

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

    sf::RenderWindow &window = WindowManager::getInstance().getWindow();
    Scene &scene = Scene::getInstance(window);
    std::vector<std::shared_ptr<Object3d> > objects = scene.getObjects();

    auto scrollView = std::make_shared<ScrollView>(
        sf::Vector2f(parentPosition.x + 20, 60),
        sf::Vector2f(310, 300),
        300.f
    );
    for (int i = 1; i < objects.size(); ++i) {
        auto objContainer = std::make_shared<Container>(
            sf::Vector2f(parentPosition.x + 20, 60 + 40 * (i - 1)),
            sf::Vector2f(310, 40)
        );
        auto objButton = std::make_shared<Button>(
            sf::Vector2f(parentPosition.x + 20, 60 + 40 * (i - 1)),
            sf::Vector2f(220, 40),
            languagePack["Object"].get<std::string>() + " " + std::to_string(i)
        );
        auto copyObjButton = std::make_shared<Button>(
            sf::Vector2f(parentPosition.x + 240, 60 + 40 * (i - 1)),
            sf::Vector2f(40, 40),
            ""
        );
        copyObjButton->setIcon("copy");
        copyObjButton->setOnClick([this, i]() {
            pendingOperations.push([this, i]() {
                sf::RenderWindow &window = WindowManager::getInstance().getWindow();
                Scene &scene = Scene::getInstance(window);
                scene.copyObjectByIndex(i);
                createUI();
                auto languagePack = LanguageManager::getInstance().getSelectedPack();
                Snackbar::getInstance().addMessage(languagePack["object_copied"]);
            });

        });
        auto deleteObjButton = std::make_shared<Button>(
            sf::Vector2f(parentPosition.x + 280, 60 + 40 * (i - 1)),
            sf::Vector2f(40, 40),
            ""
        );
        deleteObjButton->setIcon("delete");
        deleteObjButton->setOnClick([this, i]() {
            pendingOperations.push([this, i]() {
                sf::RenderWindow &window = WindowManager::getInstance().getWindow();
                Scene &scene = Scene::getInstance(window);
                scene.deleteObjectByIndex(i);
                createUI();
                auto languagePack = LanguageManager::getInstance().getSelectedPack();
                Snackbar::getInstance().addMessage(languagePack["object_deleted"]);
            });
        });

        objButton->setOnClick([this, i, &scene]() {
            scene.toggleObjectSelected(i);
            updateObjectsPropertiesMenu();
        });

        objButton->setOnHover([objects, i]() {
            objects[i]->isHovered = true;
        });
        objButton->setOnHoverOut([objects, i]() {
            objects[i]->isHovered = false;
        });

        objContainer->addComponent(objButton);
        objContainer->addComponent(copyObjButton);
        objContainer->addComponent(deleteObjButton);
        scrollView->addComponent(objContainer);
    }

    objectList.push_back(scrollView);
    objectsScroll = scrollView;
    int selectedIndex = scene.getSelectedObjectIndex();
    if (selectedIndex > 0 && selectedIndex < objects.size()) {
        currentSelectedObject = objects[selectedIndex];
        addObjectPropertiesMenu(currentSelectedObject);
    }
}

void ObjectsMenu::draw(sf::RenderWindow &window) {
    updateObjectsButtons();

    for (const auto &component: objectList) {
        component->draw(window);
    }

    for (const auto &component: objectPropertiesMenu) {
        component->draw(window);
    }
    while (!pendingOperations.empty()) {
        pendingOperations.front()();
        pendingOperations.pop();
        createUI();
    }
}

void ObjectsMenu::handleEvent(const sf::Event &event, const sf::RenderWindow &window) {
    if (objectList.empty()) {
        createUI();
    }
    for (const auto &component: objectList) {
        component->handleEvent(event, window);
    }

    for (const auto &component: objectPropertiesMenu) {
        component->handleEvent(event, window);
    }
    while (!pendingOperations.empty()) {
        pendingOperations.front()();
        pendingOperations.pop();
    }
}

void ObjectsMenu::onActivate() {
    sf::RenderWindow &window = WindowManager::getInstance().getWindow();
    Scene &scene = Scene::getInstance(window);
    scene.setObjectsEditMode(true);
    createUI();
    createObjectsPropertiesMenu();
}

void ObjectsMenu::onDeactivate() {
    objectList.clear();
    objectPropertiesMenu.clear();
    sf::RenderWindow &window = WindowManager::getInstance().getWindow();
    Scene &scene = Scene::getInstance(window);
    scene.setObjectsEditMode(false);
}

void ObjectsMenu::updateObjectsButtons() {
    sf::RenderWindow &window = WindowManager::getInstance().getWindow();
    Scene &scene = Scene::getInstance(window);
    std::vector<std::shared_ptr<Object3d> > objects = scene.getObjects();
    int selectedIndex = scene.getSelectedObjectIndex();
    if (selectedIndex > 0 && selectedIndex < objects.size()) {
        if (currentSelectedObject && objects[selectedIndex] != currentSelectedObject) {
            currentSelectedObject->selectedVertices.clear();
        }
        currentSelectedObject = objects[selectedIndex];
    }
    auto objectsList = objectsScroll->getComponents();
    for (int i = 0; i < objectsList.size(); ++i) {
        auto objContainer = std::dynamic_pointer_cast<Container>(objectsList[i]);
        auto objButton = std::dynamic_pointer_cast<Button>(objContainer->getComponents()[0]);
        if (currentSelectedObject) {
            if (objects[i + 1] && objects.size() > 1) {
                if (objects[i + 1]->isHovered) {
                    objButton->setColor(sf::Color(255, 255, 150));
                }
                if (objects[i + 1]->isSelected) {
                    objButton->setColor(sf::Color(255, 255, 100));
                }
                if (!objects[i + 1]->isHovered && !objects[i + 1]->isSelected) {
                    objButton->setColor(sf::Color::White);
                }
            }
        }
    }
    updateObjectsPropertiesMenu();
}

void ObjectsMenu::addObjectPropertiesMenu(std::shared_ptr<Object3d> object) {
    currentSelectedObject = object;
}

void ObjectsMenu::createObjectsPropertiesMenu() {
    int marginTop = 400;
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 10),
        20,
        languagePack["Properties"]
    );
    title->setBold();
    scale = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 50),
        20,
        languagePack["Scale"]
    );
    scaleX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 80),
        sf::Vector2f(90, 50),
        "X"
    );
    scaleX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        sf::RenderWindow &window = WindowManager::getInstance().getWindow();
        Scene &scene = Scene::getInstance(window);
        if (scene.selectedObjects.size() == 1) {
            currentSelectedObject->setScale(Vector3(value - scene.getObjects()[scene.getSelectedObjectIndex()]->getScale().x, 0.0f, 0.0f));
        } else {
            for (int i = 0; i < scene.selectedObjects.size(); ++i) {
                scene.getObjects()[scene.selectedObjects[i]]->setScale(Vector3(value, 0.0f, 0.0f));
                scaleX->setValue(0);
            }
        }
    }));
    scaleY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 80),
        sf::Vector2f(90, 50),
        "Y"
    );
    scaleY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        sf::RenderWindow &window = WindowManager::getInstance().getWindow();
        Scene &scene = Scene::getInstance(window);
        if (scene.selectedObjects.size() == 1) {
            currentSelectedObject->setScale(Vector3(0.0f, value - scene.getObjects()[scene.getSelectedObjectIndex()]->getScale().y, 0.0f));
        } else {
            for (int i = 0; i < scene.selectedObjects.size(); ++i) {
                scene.getObjects()[scene.selectedObjects[i]]->setScale(Vector3(0.0f, value, 0.0f));
                scaleY->setValue(0);
            }
        }
    }));
    scaleZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 80),
        sf::Vector2f(90, 50),
        "Z"
    );
    scaleZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        sf::RenderWindow &window = WindowManager::getInstance().getWindow();
        Scene &scene = Scene::getInstance(window);
        if (scene.selectedObjects.size() == 1) {
            currentSelectedObject->setScale(Vector3(0.0f, 0.0f, value - scene.getObjects()[scene.getSelectedObjectIndex()]->getScale().z));
        } else {
            for (int i = 0; i < scene.selectedObjects.size(); ++i) {
                scene.getObjects()[scene.selectedObjects[i]]->setScale(Vector3(0.0f, 0.0f, value));
                scaleZ->setValue(0);
            }
        }
    }));
    position = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 150),
        20,
        languagePack["Position"]
    );
    positionX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 180),
        sf::Vector2f(90, 50),
        "X"
    );
    positionX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        sf::RenderWindow &window = WindowManager::getInstance().getWindow();
        Scene &scene = Scene::getInstance(window);
        if (scene.selectedObjects.size() == 1) {
            currentSelectedObject->translate(Vector3(value - scene.getObjects()[scene.getSelectedObjectIndex()]->getPosition().x, 0.0f, 0.0f));
        } else {
            for (int i = 0; i < scene.selectedObjects.size(); ++i) {
                scene.getObjects()[scene.selectedObjects[i]]->translate(Vector3(value, 0.0f, 0.0f));
                positionX->setValue(0);
            }
        }
    }));
    positionY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 180),
        sf::Vector2f(90, 50),
        "Y"
    );
    positionY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        sf::RenderWindow &window = WindowManager::getInstance().getWindow();
        Scene &scene = Scene::getInstance(window);
        if (scene.selectedObjects.size() == 1) {
            currentSelectedObject->translate(Vector3(0.0f, value - scene.getObjects()[scene.getSelectedObjectIndex()]->getPosition().y, 0.0f));
        } else {
            for (int i = 0; i < scene.selectedObjects.size(); ++i) {
                scene.getObjects()[scene.selectedObjects[i]]->translate(Vector3(0.0f, value, 0.0f));
                positionY->setValue(0);
            }
        }
    }));
    positionZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 180),
        sf::Vector2f(90, 50),
        "Z"
    );
    positionZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        sf::RenderWindow &window = WindowManager::getInstance().getWindow();
        Scene &scene = Scene::getInstance(window);
        if (scene.selectedObjects.size() == 1) {
            currentSelectedObject->translate(Vector3(0.0f, 0.0f, value - scene.getObjects()[scene.getSelectedObjectIndex()]->getPosition().z));
        } else {
            for (int i = 0; i < scene.selectedObjects.size(); ++i) {
                scene.getObjects()[scene.selectedObjects[i]]->translate(Vector3(0.0f, 0.0f, value));
                positionZ->setValue(0);
            }
        }
    }));
    rotation = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 250),
        20,
        languagePack["Rotation"]
    );
    rotationX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 280),
        sf::Vector2f(90, 50),
        "X",
        0.1f
    );
    rotationX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        sf::RenderWindow &window = WindowManager::getInstance().getWindow();
        Scene &scene = Scene::getInstance(window);
        if (scene.selectedObjects.size() == 1) {
            Vector3 currentRotation = scene.getObjects()[scene.getSelectedObjectIndex()]->getRotation();
            currentSelectedObject->setRotation(Vector3(value, currentRotation.y, currentRotation.z));
        } else {
            std::cout << scene.selectedObjects.size() << std::endl;
            for (int i = 0; i < scene.selectedObjects.size(); ++i) {
                scene.getObjects()[scene.selectedObjects[i]]->rotate(value, 'x');
                rotationX->setValue(0);
            }
        }
    }));
    rotationY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, marginTop + 280),
        sf::Vector2f(90, 50),
        "Y",
        0.1f
    );
    rotationY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        sf::RenderWindow &window = WindowManager::getInstance().getWindow();
        Scene &scene = Scene::getInstance(window);
        if (scene.selectedObjects.size() == 1) {
            Vector3 currentRotation = scene.getObjects()[scene.getSelectedObjectIndex()]->getRotation();
            currentSelectedObject->setRotation(Vector3(currentRotation.x, value, currentRotation.z));
        } else {
            for (int i = 0; i < scene.selectedObjects.size(); ++i) {
                scene.getObjects()[scene.selectedObjects[i]]->rotate(value, 'y');
                rotationY->setValue(0);
            }
        }
    }));
    rotationZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, marginTop + 280),
        sf::Vector2f(90, 50),
        "Z",
        0.1f
    );
    rotationZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        sf::RenderWindow &window = WindowManager::getInstance().getWindow();
        Scene &scene = Scene::getInstance(window);
        if (scene.selectedObjects.size() == 1) {
            Vector3 currentRotation = scene.getObjects()[scene.getSelectedObjectIndex()]->getRotation();
            currentSelectedObject->setRotation(Vector3(currentRotation.x, currentRotation.y, value));
        } else {

            for (int i = 0; i < scene.selectedObjects.size(); ++i) {
                scene.getObjects()[scene.selectedObjects[i]]->rotate(value, 'z');
                rotationZ->setValue(0);
            }
        }
    }));
    color = std::make_shared<Input>(
       sf::Vector2f(parentPosition.x + 20, marginTop + 360),
       sf::Vector2f(200, 50),
       languagePack["Color"],
       languagePack["Color"]
    );
    color->setOnClick([](std::string value) {
        sf::RenderWindow &window = WindowManager::getInstance().getWindow();
        Scene &scene = Scene::getInstance(window);
        if (scene.selectedObjects.size() == 1) {
            scene.getObjects()[scene.selectedObjects[0]]->setFacesColor(value);
        }
        else {
            for (int i = 0; i < scene.selectedObjects.size(); ++i) {
                scene.getObjects()[scene.selectedObjects[i]]->setFacesColor(value);
            }
        }
    });
    combineButton = std::make_shared<Button>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 430),
        sf::Vector2f(200, 50),
        "Combine"
    );
    combineButton->setOnClick([this]() {
        pendingOperations.push([this]() {
            sf::RenderWindow &window = WindowManager::getInstance().getWindow();
            Scene &scene = Scene::getInstance(window);
            scene.createCombination();
            createUI();
        });
    });
    combineButton->setIcon("combine");
}



void ObjectsMenu::updateObjectsPropertiesMenu() {
    objectPropertiesMenu.clear();
    if (!currentSelectedObject) return;
    objectPropertiesMenu.push_back(title);
    objectPropertiesMenu.push_back(scale);
    objectPropertiesMenu.push_back(position);
    objectPropertiesMenu.push_back(rotation);

    sf::RenderWindow &window = WindowManager::getInstance().getWindow();
    Scene &scene = Scene::getInstance(window);
    if (scene.selectedObjects.size() == 1) {
        Vector3 currentScale = currentSelectedObject->getScale();
        Vector3 currentPosition = currentSelectedObject->getPosition();
        Vector3 currentRotation = currentSelectedObject->getRotation();

        if (scaleX->lastUpdatedValue != currentScale.x) {
            scaleX->setValue(currentScale.x);
        }
        if (scaleY->lastUpdatedValue != currentScale.y) {
            scaleY->setValue(currentScale.y);
        }
        if (scaleZ->lastUpdatedValue != currentScale.z) {
            scaleZ->setValue(currentScale.z);
        }

        if (positionX->lastUpdatedValue != currentPosition.x) {
            positionX->setValue(currentPosition.x);
        }
        if (positionY->lastUpdatedValue != currentPosition.y) {
            positionY->setValue(currentPosition.y);
        }
        if (positionZ->lastUpdatedValue != currentPosition.z) {
            positionZ->setValue(currentPosition.z);
        }

        if (rotationX->lastUpdatedValue != currentRotation.x) {
            rotationX->setValue(currentRotation.x);
        }
        if (rotationY->lastUpdatedValue != currentRotation.y) {
            rotationY->setValue(currentRotation.y);
        }
        if (rotationZ->lastUpdatedValue != currentRotation.z) {
            rotationZ->setValue(currentRotation.z);
        }
    }
    else {
        std::cout << scene.selectedObjects.size() << std::endl;
        if (scaleX->lastUpdatedValue != 0.0f) {
            scaleX->setValue(0);
        }
        if (scaleY->lastUpdatedValue != 0.0f) {
            scaleY->setValue(0);
        }
        if (scaleZ->lastUpdatedValue != 0.0f) {
            scaleZ->setValue(0);
        }

        if (positionX->lastUpdatedValue != 0.0f) {
            positionX->setValue(0);
        }
        if (positionY->lastUpdatedValue != 0.0f) {
            positionY->setValue(0);
        }
        if (positionZ->lastUpdatedValue != 0.0f) {
            positionZ->setValue(0);
        }

        if (rotationX->lastUpdatedValue != 0.0f) {
            rotationX->setValue(0);
        }
        if (rotationY->lastUpdatedValue != 0.0f) {
            rotationY->setValue(0);
        }
        if (rotationZ->lastUpdatedValue != 0.0f) {
            rotationZ->setValue(0);
        }
    }
    if (color->lastUpdatedValue != "") {
        color->setValue("");
    }
    objectPropertiesMenu.push_back(scaleX);
    objectPropertiesMenu.push_back(scaleY);
    objectPropertiesMenu.push_back(scaleZ);

    objectPropertiesMenu.push_back(positionX);
    objectPropertiesMenu.push_back(positionY);
    objectPropertiesMenu.push_back(positionZ);
    objectPropertiesMenu.push_back(rotationX);
    objectPropertiesMenu.push_back(rotationY);
    objectPropertiesMenu.push_back(rotationZ);
    objectPropertiesMenu.push_back(color);
    if (scene.selectedObjects.size() > 1) {
        objectPropertiesMenu.push_back(combineButton);
    }
}
