#include "VerticesMenu.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../core/ui/Container.hpp"
#include "../../utils/WindowManager.hpp"
#include "../../core/ui/NumberInput.hpp"
#include "../../core/ui/Input.hpp"
#include "../../core/ui/ScrollView.hpp"
#include "../../core/ui/Expandable.hpp"
#include "../../core/ui/Snackbar.hpp"
#include "../../utils/LanguageManager.hpp"

VerticesMenu::VerticesMenu() : currentSelectedObject(nullptr) {}

void VerticesMenu::createUI() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    objectList.clear();
    components.clear();
    auto title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 20),
        20,
        languagePack["vertices"]
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
        objButton->setExpanded(objects[i] == currentSelectedObject);

        int j = 0;
        for (auto vertex : objects[i]->getVertices()) {
            auto vertexContainer = std::make_shared<Container>(
                sf::Vector2f(parentPosition.x + 40, 60 + 30 * (j + i)),
                sf::Vector2f(250, 30)
            );
            auto vertexButton = std::make_shared<Button>(
                sf::Vector2f(parentPosition.x + 40, 60 + 30 * (j + i)),
                sf::Vector2f(240, 30),
                 languagePack["Vertex"].get<std::string>() + " " + std::to_string(j + 1)
            );
            if (objects[i]->isVertexSelected(i)) {
                vertexButton->setColor(sf::Color(255, 255, 100));
            }

            vertexButton->setOnClick([this, j]() {
                auto& selectedVertices = currentSelectedObject->selectedVertices;
                auto it = std::find(selectedVertices.begin(), selectedVertices.end(), j);
                if (it != selectedVertices.end()) {
                    selectedVertices.erase(it);
                } else {
                    selectedVertices.push_back(j);
                }
                updateVertexPropertiesMenu();
            });
            vertexButton->setOnHover([this, j]() {
                auto& hoveredVertices = currentSelectedObject->hoveredVertices;
                auto it = std::find(hoveredVertices.begin(), hoveredVertices.end(), j);
                if (it == hoveredVertices.end()) {
                    hoveredVertices.push_back(j);
                }
            });
            vertexButton->setOnHoverOut([this, j]() {
                auto& hoveredVertices = currentSelectedObject->hoveredVertices;
                auto it = std::find(hoveredVertices.begin(), hoveredVertices.end(), j);
                if (it != hoveredVertices.end()) {
                    hoveredVertices.erase(it);
                }
            });
            auto deleteVertexButton = std::make_shared<Button>(
                sf::Vector2f(parentPosition.x + 280, 60 + 30 * (j + i)),
                sf::Vector2f(30, 30),
                ""
            );
            deleteVertexButton->setIcon("delete");
            deleteVertexButton->setOnClick([this, j]() {
                pendingOperations.push([this, j]() {
                    currentSelectedObject->deleteVertex(j);
                    createUI();
                    auto languagePack = LanguageManager::getInstance().getSelectedPack();
                    Snackbar::getInstance().addMessage(languagePack["vertex_deleted"]);
                });
            });
            vertexContainer->addComponent(vertexButton);
            vertexContainer->addComponent(deleteVertexButton);
            objButton->addContent(vertexContainer);
            j++;
        }

        scrollView->addComponent(objButton);
    }
    objectsScroll = scrollView;
    objectList.push_back(scrollView);

    int selectedIndex = scene.getSelectedObjectIndex();
    if (selectedIndex > 1 && selectedIndex < objects.size()) {
        currentSelectedObject = objects[selectedIndex];

    }
}


void VerticesMenu::draw(sf::RenderWindow &window) {
    if (objectList.empty()) {
        createUI();
    }

    for (const auto& component : objectList) {
        component->draw(window);
    }

    for (const auto& component : vertexPropertiesMenu) {
        component->draw(window);
    }
    while (!pendingOperations.empty()) {
        pendingOperations.front()();
        pendingOperations.pop();
    }
}

void VerticesMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (objectList.empty()) {
        createUI();
    }
    updateVerticesButtons();
    for (const auto& component : objectList) {
        component->handleEvent(event, window);
    }
    for (const auto& component : vertexPropertiesMenu) {
        component->handleEvent(event, window);
    }
    while (!pendingOperations.empty()) {
        pendingOperations.front()();
        pendingOperations.pop();
    }
}

void VerticesMenu::onActivate() {
    createUI();
    createVertexPropertiesMenu();
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    scene.setVerticesEditMode(true);
}

void VerticesMenu::onDeactivate() {
    objectList.clear();
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    scene.setVerticesEditMode(false);
}

void VerticesMenu::updateVerticesButtons() {
    sf::RenderWindow& window = WindowManager::getInstance().getWindow();
    Scene& scene = Scene::getInstance(window);
    std::vector<std::shared_ptr<Object3d>> objects = scene.getObjects();
    int selectedIndex = scene.getSelectedObjectIndex();
    if (selectedIndex > 0 && selectedIndex < objects.size()) {
        if (currentSelectedObject && objects[selectedIndex] != currentSelectedObject) {
            currentSelectedObject->selectedVertices.clear();
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
            for (const auto& vertexComponent: content) {
                auto vertexContainer = std::dynamic_pointer_cast<Container>(vertexComponent);
                auto vertexButton = std::dynamic_pointer_cast<Button>(vertexContainer->getComponents()[0]);
                if (vertexButton) {
                    if (objects[i + 1]->isVertexHovered(j)) {
                        vertexButton->setColor(sf::Color(255, 255, 150));
                    }
                    if (objects[i + 1]->isVertexSelected(j)) {
                        vertexButton->setColor(sf::Color(255, 255, 100));
                    }
                    if (!objects[i + 1]->isVertexHovered(j) && !objects[i + 1]->isVertexSelected(j)) {
                        vertexButton->setColor(sf::Color::White);
                    }
                }
                j++;
            }
        }
    }
    updateVertexPropertiesMenu();
}

void VerticesMenu::createVertexPropertiesMenu() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();
    addVertexButton = std::make_shared<Button>(
    sf::Vector2f(parentPosition.x + 190,  20),
           sf::Vector2f(120, 30),
        languagePack["add_vertex"]
       );
    addVertexButton->setIcon("vertex");
    addVertexButton->setPosition({parentPosition.x + 330 - addVertexButton->getWidth(), addVertexButton->getPosition().y});
    addVertexButton->setOnClick([this]() {
        currentSelectedObject->addVertex();
        createUI();
        auto languagePack = LanguageManager::getInstance().getSelectedPack();
        Snackbar::getInstance().addMessage(languagePack["vertex_added"]);
    });
    title = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20,  410),
        20,
        languagePack["Vertex Properties"]
    );
    title->setBold();
    position = std::make_shared<Text>(
        sf::Vector2f(parentPosition.x + 20, 450),
        20,
        languagePack["Position"]
    );
    positionX = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 20, 480),
        sf::Vector2f(90, 50),
        "X"
    );
    positionX->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        if (currentSelectedObject->selectedVertices.size() == 1) {
            int selectedVertexIndex = currentSelectedObject->selectedVertices[0];
            currentSelectedObject->updateVertex(selectedVertexIndex,
                {   value,
                    currentSelectedObject->getVertices()[selectedVertexIndex].y,
                    currentSelectedObject->getVertices()[selectedVertexIndex].z
                });
        }
        else {
            for (auto vertex: currentSelectedObject->selectedVertices) {
                currentSelectedObject->updateVertex(vertex,
                    {
                        currentSelectedObject->getVertices()[vertex].x + value,
                        currentSelectedObject->getVertices()[vertex].y,
                        currentSelectedObject->getVertices()[vertex].z
                    });
            }
            positionX->setValue(0.0f);
        }
    }));
    positionY = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 130, 480),
        sf::Vector2f(90, 50),
        "Y"
    );
    positionY->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        if (currentSelectedObject->selectedVertices.size() == 1) {
            int selectedVertexIndex = currentSelectedObject->selectedVertices[0];
            currentSelectedObject->updateVertex(selectedVertexIndex,
                {   currentSelectedObject->getVertices()[selectedVertexIndex].x,
                    value,
                    currentSelectedObject->getVertices()[selectedVertexIndex].z
                });
        }
        else {
            for (auto vertex: currentSelectedObject->selectedVertices) {
                currentSelectedObject->updateVertex(vertex,
                    {
                        currentSelectedObject->getVertices()[vertex].x,
                        currentSelectedObject->getVertices()[vertex].y + value,
                        currentSelectedObject->getVertices()[vertex].z
                    });
            }
            positionY->setValue(0.0f);
        }
    }));
    positionZ = std::make_shared<NumberInput>(
        sf::Vector2f(parentPosition.x + 240, 480),
        sf::Vector2f(90, 50),
        "Z"
    );
    positionZ->setOnClick(static_cast<std::function<void(float)>>([this](float value) {
        if (currentSelectedObject->selectedVertices.size() == 1) {
            int selectedVertexIndex = currentSelectedObject->selectedVertices[0];
            currentSelectedObject->updateVertex(selectedVertexIndex,
                {   currentSelectedObject->getVertices()[selectedVertexIndex].x,
                    currentSelectedObject->getVertices()[selectedVertexIndex].y,
                    value
                });
        }
        else {
            for (auto vertex: currentSelectedObject->selectedVertices) {
                currentSelectedObject->updateVertex(vertex,
                    {
                        currentSelectedObject->getVertices()[vertex].x,
                        currentSelectedObject->getVertices()[vertex].y,
                        currentSelectedObject->getVertices()[vertex].z + value
                    });
            }
            positionZ->setValue(0.0f);
        }
    }));
    createEdgeButton = std::make_shared<Button>(
        sf::Vector2f(parentPosition.x + 20, 550),
                sf::Vector2f(200, 50),
                languagePack["Create Edge"]
            );
    createEdgeButton->setIcon("edge");
    createEdgeButton->setOnClick([this]() {
        currentSelectedObject->createEdge();
    });
    createFaceButton = std::make_shared<Button>(
        sf::Vector2f(parentPosition.x + 20, 550),
                sf::Vector2f(200, 50),
                languagePack["Create Face"]
            );
    createFaceButton->setIcon("face");
    createFaceButton->setOnClick([this]() {
        currentSelectedObject->createFace();
    });
}

void VerticesMenu::updateVertexPropertiesMenu() {
    vertexPropertiesMenu.clear();
    if (currentSelectedObject) {
        vertexPropertiesMenu.push_back(addVertexButton);
    }
    else return;
    if (currentSelectedObject->selectedVertices.empty()) return;
    vertexPropertiesMenu.push_back(title);
    vertexPropertiesMenu.push_back(position);
    if (currentSelectedObject->selectedVertices.size() == 1) {
        Vector3 lastValue = currentSelectedObject->getVertices()[currentSelectedObject->selectedVertices[0]];
        if (positionX->lastUpdatedValue != lastValue.x) {
            positionX->setValue(lastValue.x);
        }
        if (positionY->lastUpdatedValue != lastValue.y) {
            positionY->setValue(lastValue.y);
        }
        if (positionZ->lastUpdatedValue != lastValue.z) {
            positionZ->setValue(lastValue.z);
        }
    }
    else {
        if (positionX->lastUpdatedValue != 0.0f) {
            positionX->setValue(0.0f);
        }
        if (positionY->lastUpdatedValue != 0.0f) {
            positionY->setValue(0.0f);
        }
        if (positionZ->lastUpdatedValue != 0.0f) {
            positionZ->setValue(0.0f);
        }
    }

    vertexPropertiesMenu.push_back(positionX);
    vertexPropertiesMenu.push_back(positionY);
    vertexPropertiesMenu.push_back(positionZ);

    if (currentSelectedObject->selectedVertices.size() == 2 && !currentSelectedObject->hasEdgeFromSelection()) {
        vertexPropertiesMenu.push_back(createEdgeButton);
    }
    else if (currentSelectedObject->selectedVertices.size() == 3 && !currentSelectedObject->hasFaceFromSelection()) {
        vertexPropertiesMenu.push_back(createFaceButton);
    }
}
