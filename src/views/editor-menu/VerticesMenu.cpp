#include "VerticesMenu.hpp"
#include "../../core/feature/Scene.hpp"
#include "../../utils/WindowManager.hpp"
#include "../../core/ui/NumberInput.hpp"
#include "../../core/ui/Input.hpp"
#include "../../core/ui/ScrollView.hpp"
#include "../../core/ui/Expandable.hpp"
#include "../../utils/LanguageManager.hpp"

VerticesMenu::VerticesMenu() : currentSelectedObject(nullptr) {}

void VerticesMenu::createUI() {
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
        for (auto vertex : objects[i]->getVertices()) {
            sf::Color btnColor = sf::Color::White;
            if (currentSelectedObject) {
                if (currentSelectedObject->isVertexSelected(j)) btnColor = sf::Color(255, 255,100);
            }
            auto vertexButton = std::make_shared<Button>(
            sf::Vector2f(parentPosition.x + 40, 60 + 30 * (i - 1)),
            sf::Vector2f(180, 30),
             languagePack["Vertex"].get<std::string>() + " " + std::to_string(j + 1),
            btnColor
        );

            vertexButton->setOnClick([this, j, vertexButton]() {
                auto& selectedVertices = currentSelectedObject->selectedVertices;
                auto it = std::find(selectedVertices.begin(), selectedVertices.end(), j);
                if (it != selectedVertices.end()) {
                    selectedVertices.erase(it);
                    vertexButton->setColor(sf::Color::White);
                } else {
                    selectedVertices.push_back(j);
                    vertexButton->setColor(sf::Color(255, 255, 100));
                }
                addVertexPropertiesMenu();
            });
            objButton->addContent(vertexButton);
            j++;
        }

        scrollView->addComponent(objButton);
    }

    objectList.push_back(scrollView);

    int selectedIndex = scene.getSelectedObjectIndex();
    if (selectedIndex > 0 && selectedIndex < objects.size()) {
        currentSelectedObject = objects[selectedIndex];
        if (currentSelectedObject->selectedVertices.size()) {
            addVertexPropertiesMenu();
        }
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
}

void VerticesMenu::handleEvent(const sf::Event& event, const sf::RenderWindow& window) {
    if (objectList.empty()) {
        createUI();
    }

    for (const auto& component : objectList) {
        component->handleEvent(event, window);
    }
    for (const auto& component : vertexPropertiesMenu) {
        component->handleEvent(event, window);
    }
}

void VerticesMenu::onActivate() {
    createUI();
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

void VerticesMenu::addVertexPropertiesMenu() {
    auto languagePack = LanguageManager::getInstance().getSelectedPack();

    vertexPropertiesMenu.clear();
    int marginTop = 400;
    if (currentSelectedObject->selectedVertices.size() == 0) {
        auto addVertexButton = std::make_shared<Button>(
   sf::Vector2f(parentPosition.x + 20, marginTop + 350),
           sf::Vector2f(120, 50),
           languagePack["Add"]
       );

        addVertexButton->setOnClick([this]() {
            currentSelectedObject->addVertex();
            createUI();
        });
        vertexPropertiesMenu.push_back(addVertexButton);
    }

    if (currentSelectedObject->selectedVertices.size() == 1) {
        int selectedVertexIndex = currentSelectedObject->selectedVertices[0];

        auto title = std::make_shared<Text>(
            sf::Vector2f(parentPosition.x + 20, marginTop + 10),
            20,
            languagePack["Vertex Properties"]
        );
        vertexPropertiesMenu.push_back(title);

    ///////////////////////////////////////////////////////////////////

        auto position = std::make_shared<Text>(
            sf::Vector2f(parentPosition.x + 20, marginTop + 50),
            20,
            languagePack["Position"]
        );
        vertexPropertiesMenu.push_back(position);


        auto positionX = std::make_shared<NumberInput>(
            sf::Vector2f(parentPosition.x + 20, marginTop + 80),
            sf::Vector2f(90, 50),
            "X"
        );
        positionX->setValue(currentSelectedObject->getVertices()[selectedVertexIndex].x);

        positionX->setOnClick(static_cast<std::function<void(float)>>([this, selectedVertexIndex](float value) {
            currentSelectedObject->updateVertex(selectedVertexIndex,
                {
                    value,
                    currentSelectedObject->getVertices()[selectedVertexIndex].y,
                    currentSelectedObject->getVertices()[selectedVertexIndex].z
                });
        }));
        vertexPropertiesMenu.push_back(positionX);

        auto positionY = std::make_shared<NumberInput>(
            sf::Vector2f(parentPosition.x + 130, marginTop + 80),
            sf::Vector2f(90, 50),
            "Y"
        );
        positionY->setValue(currentSelectedObject->getVertices()[selectedVertexIndex].y);

        positionY->setOnClick(static_cast<std::function<void(float)>>([this, selectedVertexIndex](float value) {
            currentSelectedObject->updateVertex(selectedVertexIndex,
                {
                    currentSelectedObject->getVertices()[selectedVertexIndex].x,
                    value,
                    currentSelectedObject->getVertices()[selectedVertexIndex].z
                });
        }));
        vertexPropertiesMenu.push_back(positionY);

        auto positionZ = std::make_shared<NumberInput>(
            sf::Vector2f(parentPosition.x + 240, marginTop + 80),
            sf::Vector2f(90, 50),
            "Z"
        );
        positionZ->setValue(currentSelectedObject->getVertices()[selectedVertexIndex].z);

        positionZ->setOnClick(static_cast<std::function<void(float)>>([this, selectedVertexIndex](float value) {
            currentSelectedObject->updateVertex(selectedVertexIndex,
                {
                    currentSelectedObject->getVertices()[selectedVertexIndex].x,
                    currentSelectedObject->getVertices()[selectedVertexIndex].y,
                    value
                });
        }));
        vertexPropertiesMenu.push_back(positionZ);

        auto deleteVertexButton = std::make_shared<Button>(
        sf::Vector2f(parentPosition.x + 210, marginTop + 150),
                sf::Vector2f(120, 50),
                languagePack["Delete"]
            );

        deleteVertexButton->setOnClick([this, selectedVertexIndex]() {
            currentSelectedObject->deleteVertex(selectedVertexIndex);
        });

        vertexPropertiesMenu.push_back(deleteVertexButton);
    }
    else if (currentSelectedObject->selectedVertices.size() == 2) {
        auto createEdgeButton = std::make_shared<Button>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 150),
                sf::Vector2f(200, 50),
                languagePack["Create Edge"]
            );

        createEdgeButton->setOnClick([this]() {
            currentSelectedObject->createEdge();
        });

        vertexPropertiesMenu.push_back(createEdgeButton);
    }
    else if (currentSelectedObject->selectedVertices.size() == 3) {
        auto createFaceButton = std::make_shared<Button>(
        sf::Vector2f(parentPosition.x + 20, marginTop + 150),
                sf::Vector2f(200, 50),
                languagePack["Create Face"]
            );

        createFaceButton->setOnClick([this]() {
            currentSelectedObject->createFace();
        });

        vertexPropertiesMenu.push_back(createFaceButton);
    }



}
