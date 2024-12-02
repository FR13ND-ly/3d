#ifndef SCENE_HPP
#define SCENE_HPP

#include "../objects/Object3d.hpp"
#include "../objects/Ground.hpp"
#include "Camera.hpp"
#include <vector>
#include <memory>

class Scene {
public:
    Scene() : camera(800, 600, 90.0f, 0.1f, 100.0f) {
        Ground ground = Ground(500.f);
        // addObject(std::shared_ptr<Object3d>(new Ground(100.f)));
    }

    void addObject(std::shared_ptr<Object3d> object) {
        objects.push_back(object);
    }

    const std::vector<std::shared_ptr<Object3d>>& getObjects() const {
        return objects;
    }

    void setCamera(const Camera& camera) {
        this->camera = camera;
    }

    const Camera& getCamera() const {
        return camera;
    }

    Camera& getCamera() {
        return camera;
    }

private:
    std::vector<std::shared_ptr<Object3d>> objects; // List of 3D objects in the scene
    Camera camera;
};

#endif
