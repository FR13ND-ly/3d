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
        Vector3 forward = getForwardVector();  // Use the rotation to get the forward vector
        Vector3 right = getRightVector();      // Get the right vector for lateral movement
        position = position + forward * direction.z + right * direction.x;
    }

    void rotate(float deltaX, float deltaY) {
        rotation.x += deltaY * CAMERA_ROTATION_SENSITIVITY;
        rotation.y += deltaX * CAMERA_ROTATION_SENSITIVITY;

        // Clamp pitch (x rotation) to avoid gimbal lock
        if (rotation.x > 89.0f) rotation.x = 89.0f;
        if (rotation.x < -89.0f) rotation.x = -89.0f;
    }

    Vector3 getPosition() const {
        return position;
    }

    Vector3 getRotation() const {
        return rotation;
    }

    Vector3 getForwardVector() const {
        return Vector3(
            cos(rotation.y) * cos(rotation.x),
            sin(rotation.x),
            sin(rotation.y) * cos(rotation.x)
        ).normalized();
    }

    Vector3 getRightVector() const {
        return Vector3(
            cos(rotation.y + 90.0f),
            0.0f,
            sin(rotation.y + 90.0f)
        ).normalized();
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
};

#endif
