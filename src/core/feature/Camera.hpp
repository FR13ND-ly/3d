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
          yaw(atan2(position.z, position.x)),
          pitch(asin(position.y / position.length())),
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
        updateYawPitchFromPosition();
    }

    void updateAspectRatio(float windowWidth, float windowHeight) {
        this->windowWidth = windowWidth;
        this->windowHeight = windowHeight;
    }

    void orbitPitch(float pitchDelta) {
        pitch += pitchDelta * DEG_TO_RAD;

        const float maxPitch = 3.14159265359f / 2.0f - 0.01f;
        const float minPitch = -3.14159265359f / 2.0f + 0.01f;
        if (pitch > maxPitch) pitch = maxPitch;
        if (pitch < minPitch) pitch = minPitch;

        float radius = position.length();
        position.x = radius * cos(pitch) * cos(yaw);
        position.y = radius * sin(pitch);
        position.z = radius * cos(pitch) * sin(yaw);

    lookAtCenter();
}

// Orbit around the center (left-right rotation)
    void orbitYaw(float yawDelta) {
        // Get the current position relative to the center
        Vector3 direction = position;

        // Remove the y-component from the direction to ensure we stay on the horizontal plane
        direction.y = 0.0f;

        // Normalize the direction for a consistent radius
        direction = direction.normalized();

        // Apply the yawDelta to the direction's x and z components (around the y-axis)
        float radius = position.length(); // The radius from the center (ignores height)
        float angle = atan2(direction.z, direction.x); // Get the current angle in the xz-plane

        // Update the angle with yawDelta
        angle += yawDelta * DEG_TO_RAD;

        // Calculate the new x and z position
        position.x = radius * cos(angle);
        position.z = radius * sin(angle);

        // Keep the y position the same as the original (so it stays level with the target)
        // and recalculate pitch and yaw to keep the camera upright
        position.y = position.y; // No change in the y-coordinate

        // Update the camera's rotation to always look at the target (usually (0, 0, 0))
        lookAtCenter();
    }



    void lookAtCenter() {
        Vector3 direction = (Vector3(0.0f, 0.0f, 0.0f) - position).normalized();

        rotation.y = atan2(direction.x, direction.x); // Yaw
        rotation.x = -asin(direction.y);              // Pitch
        rotation.z = 0.0f;                            // Roll (keep at zero)

        std::cout << rotation.x << " " << rotation.y << " " << rotation.z << std::endl;
    }


private:
    Vector3 position;
    Vector3 rotation;

    float yaw, pitch; // Persistent yaw and pitch

    float windowWidth, windowHeight;
    float fov, near, far;

    static constexpr float CAMERA_ROTATION_SENSITIVITY = 0.05f;
    static constexpr float DEG_TO_RAD = 3.14159265359f / 180.0f;

    void updateYawPitchFromPosition() {
        float radius = position.length();
        yaw = atan2(position.z, position.x);
        pitch = asin(position.y / radius);
    }
};

#endif
