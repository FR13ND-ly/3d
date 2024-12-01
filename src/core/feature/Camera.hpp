#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../../utils/Vector3.hpp"

class Camera {
public:
    Camera() : position(0, 0, -5) {}

    Vector3 project(const Vector3& point) const {
        float zInverse = 1.0f / (point.z + 5.0f);
        return Vector3(point.x * zInverse, point.y * zInverse, zInverse);
    }

private:
    Vector3 position;
};

#endif
