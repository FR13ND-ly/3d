#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../../utils/Vector3.hpp"
#include "../../utils/Matrix4.hpp"

class Camera {
public:
    Camera(float windowWidth, float windowHeight, float fov = 90.0f, float near = 1.0f, float far = 1000.0f)
        : position(0.0f, 0.0f, -5.0f),
          rotation(0.0f, 0.0f, 0.0f),
          windowWidth(windowWidth),
          windowHeight(windowHeight),
          fov(fov),
          near(near),
          far(far) {
    }

    Matrix4 getViewMatrix() const {
        Matrix4 translation = Matrix4::translation(-position.x, -position.y, -position.z);
        Matrix4 rotationX = Matrix4::rotationX(-rotation.x);
        Matrix4 rotationY = Matrix4::rotationY(-rotation.y);
        Matrix4 rotationZ = Matrix4::rotationZ(-rotation.z);

        return rotationZ * rotationY * rotationX * translation;
    }

    Matrix4 getProjectionMatrix() const {
        float aspectRatio = windowWidth / windowHeight;
        return Matrix4::perspective(fov, aspectRatio, near, far);
    }

    void move(const Vector3 &direction) {
        position = position + direction;
    }

    void rotate(float deltaX, float deltaY) {
        rotation.x += deltaY * CAMERA_ROTATION_SENSITIVITY;
        rotation.y += deltaX * CAMERA_ROTATION_SENSITIVITY;
    }

    Vector3 position;
    Vector3 rotation;

private:
    float windowWidth, windowHeight;
    float fov, near, far;
    static constexpr float CAMERA_ROTATION_SENSITIVITY = 0.005f;
};

#endif
