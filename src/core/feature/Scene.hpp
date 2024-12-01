#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include "../../utils/Object3D.hpp"
#include "Camera.hpp"

class Scene {
public:
    Scene() {
        camera = new Camera();
    }

    ~Scene() {
        delete camera;
        for (auto object : objects) {
            delete object;
        }
    }

    void addObject(Object3d* object) {
        objects.push_back(object);
    }

    std::vector<Object3d*> getObjects() const {
        return objects;
    }

    Camera& getCamera() const {
        return *camera;
    }

    void update() {
        // Update objects or camera if needed
    }

private:
    std::vector<Object3d*> objects;
    Camera* camera;
};

#endif
