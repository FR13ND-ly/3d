#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../../utils/Vector3.hpp"
#include "../../utils/Vector3.hpp"
#include "../../utils/Matrix4.hpp"
#include <cmath>
#include <iostream>
#include <array>

class Camera {
public:
    Camera(float windowWidth, float windowHeight, float fov = 90.0f, float near = 1.0f, float far = 1000.0f)
        : position(0.0f, -1.0f, -10.0f),
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
        return rotationY * rotationZ * rotationX  * translation;
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


    Vector3 getCameraDirection() const {
        return Vector3(
            std::cos(rotation.x) * std::sin(rotation.y), // X component
            -std::sin(rotation.x),                      // Y component
            std::cos(rotation.x) * std::cos(rotation.y)  // Z component
        ).normalized();
    }


    void rotateVertical(float deltaY) {
        float smoothedDeltaY = deltaY * CAMERA_ROTATION_SENSITIVITY;

        // Determine the primary axis of the camera's orientation
        Vector3 forward = getForwardVector();
        float xComponent = forward.x;
        float zComponent = forward.z;
        float total = std::abs(xComponent) + std::abs(zComponent);

        // Calculate the blend factor between x and z
        float blendFactor = std::abs(xComponent) / total;

        // Determine the sign of the adjustment based on the direction the camera is facing
        float pitchSign = (zComponent >= 0) ? 1.0f : -1.0f;
        float rollSign = (xComponent >= 0) ? -1.0f : 1.0f;

        // Calculate the new pitch and roll angles based on the blend factor and direction
        float newPitch = rotation.x + smoothedDeltaY * (1.0f - blendFactor) * pitchSign;
        float newRoll = rotation.z + smoothedDeltaY * blendFactor * rollSign;

        // Clamp the pitch and roll to prevent gimbal lock
        if (newPitch > 89.0f * DEG_TO_RAD) newPitch = 89.0f * DEG_TO_RAD;
        if (newPitch < -89.0f * DEG_TO_RAD) newPitch = -89.0f * DEG_TO_RAD;

        if (newRoll > 89.0f * DEG_TO_RAD) newRoll = 89.0f * DEG_TO_RAD;
        if (newRoll < -89.0f * DEG_TO_RAD) newRoll = -89.0f * DEG_TO_RAD;

        // Update the pitch and roll rotations
        rotation.x = newPitch;
        rotation.z = newRoll;
    }

    void rotateHorizontal(float deltaX) {
        float smoothedDeltaX = deltaX * CAMERA_ROTATION_SENSITIVITY;

        rotation.y += smoothedDeltaX;
        if (rotation.y > 360.0f * DEG_TO_RAD) rotation.y -= 360.0f * DEG_TO_RAD;
        if (rotation.y < 0.0f) rotation.y += 360.0f * DEG_TO_RAD;

        rotation.x = 0.0f;
        rotation.z = 0.0f;
    }


    void orbitYaw(float yawDelta) {
        // Project position onto the XZ plane
        Vector3 positionXZ(position.x, 0.0f, position.z);
        float radius = positionXZ.length();

        // Determine the current angle of rotation in the XZ plane
        float angle = std::atan2(position.z, position.x);

        // Apply yaw delta
        angle += yawDelta * DEG_TO_RAD;

        // Calculate the new XZ plane position
        float newX = radius * std::cos(angle);
        float newZ = radius * std::sin(angle);

        // Preserve the original Y height
        position.x = newX;
        position.z = newZ;

        // Maintain camera orientation towards the center
        lookAtHorizontalCenter();
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

    void lookAtHorizontalCenter() {
        Vector3 center(0.0f, 0.0f, 0.0f);
        Vector3 direction = center - position;

        // Calculate the new horizontal rotation (yaw)
        float newHorizontalRotation = std::atan2(direction.x, direction.z);

        // Preserve the magnitude of the current vertical rotation (pitch and roll)
        float currentVerticalRotationMagnitude = std::sqrt(rotation.x * rotation.x + rotation.z * rotation.z);

        // Determine blending factors based on the camera's orientation
        float forwardX = std::cos(newHorizontalRotation); // Contribution towards X-axis
        float forwardZ = std::sin(newHorizontalRotation); // Contribution towards Z-axis
        float total = std::abs(forwardX) + std::abs(forwardZ);

        // Blend the vertical rotation magnitude between rotation.x and rotation.z
        float blendFactorX = std::abs(forwardX) / total;
        float blendFactorZ = std::abs(forwardZ) / total;

        rotation.x = currentVerticalRotationMagnitude * forwardZ * blendFactorX;
        rotation.z = currentVerticalRotationMagnitude * forwardX * blendFactorZ;

        // Update horizontal rotation (yaw)
        rotation.y = newHorizontalRotation;

        // Clamp vertical rotations to prevent gimbal lock
        if (rotation.x > 89.0f * DEG_TO_RAD) rotation.x = 89.0f * DEG_TO_RAD;
        if (rotation.x < -89.0f * DEG_TO_RAD) rotation.x = -89.0f * DEG_TO_RAD;

        if (rotation.z > 89.0f * DEG_TO_RAD) rotation.z = 89.0f * DEG_TO_RAD;
        if (rotation.z < -89.0f * DEG_TO_RAD) rotation.z = -89.0f * DEG_TO_RAD;
    }


private:
    Vector3 position;
    Vector3 rotation;

    float windowWidth, windowHeight;
    float fov, near, far;

    static constexpr float CAMERA_ROTATION_SENSITIVITY = 0.1f;
    static constexpr float DEG_TO_RAD = 3.14159265359f / 180.0f;
};

#endif
