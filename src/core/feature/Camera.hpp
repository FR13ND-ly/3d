#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../../utils/Vector3.hpp"
#include "../../utils/Matrix4.hpp"
#include <cmath>
#include <iostream>
#include <array>

class Camera {
public:
    Camera(float windowWidth, float windowHeight, float fov = 90.0f, float near = 1.0f, float far = 1000.0f)
        : position(0.0f, 0.0f, -10.0f),
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

        return rotationY * rotationX * translation;
    }

    Matrix4 getProjectionMatrix() const {
        float aspectRatio = windowWidth / windowHeight;
        float fovRadians = fov * (3.14159265359f / 180.0f);
        return Matrix4::perspective(fovRadians, aspectRatio, near, far);
    }

    void move(const Vector3 &direction) {
        Vector3 forward = getForwardVector();
        Vector3 right = getRightVector();
        Vector3 up = getUpVector();

        position = position + (forward * direction.z + right * direction.x - up * direction.y);
    }

    void rotate(float deltaX, float deltaY) {
        rotation.x += deltaY * CAMERA_ROTATION_SENSITIVITY;
        rotation.y += deltaX * CAMERA_ROTATION_SENSITIVITY;

        // Clamp pitch to avoid gimbal lock
        if (rotation.x > 89.0f * DEG_TO_RAD) rotation.x = 89.0f * DEG_TO_RAD;
        if (rotation.x < -89.0f * DEG_TO_RAD) rotation.x = -89.0f * DEG_TO_RAD;

        rotation.y = std::fmod(rotation.y, 360.0f * DEG_TO_RAD);
    }

    Vector3 getPosition() const {
        return position;
    }

    Vector3 getRotation() const {
        return rotation;
    }

    Vector3 getForwardVector() const {
        return Vector3(
            cos(rotation.x) * sin(rotation.y),
            -sin(rotation.x),
            cos(rotation.x) * cos(rotation.y)
        ).normalized();
    }

    Vector3 getRightVector() const {
        return Vector3(
            sin(rotation.y - 3.14159265f / 2.0f),
            0.0f,
            cos(rotation.y - 3.14159265f / 2.0f)
        ).normalized();
    }

    Vector3 getUpVector() const {
        Vector3 forward = getForwardVector();
        Vector3 right = getRightVector();
        return right.cross(forward).normalized();
    }

    void setPosition(const Vector3& newPosition) {
        position = newPosition;
    }

private:
    Vector3 position;
    Vector3 rotation;

    float windowWidth, windowHeight;
    float fov, near, far;

    static constexpr float CAMERA_ROTATION_SENSITIVITY = 0.05f;
    static constexpr float DEG_TO_RAD = 3.14159265359f / 180.0f;
};

#endif
