#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "../../utils/math/Vector3.hpp"
#include "../../utils/math/Matrix4.hpp"
#include "../../utils/math/Vector4.hpp"


class Camera {
public:
    Camera(float windowWidth, float windowHeight, float fov = 90.0f, float nearClip = 1.0f, float farClip = 1000.0f);

    Matrix4 getViewMatrix() const;
    Matrix4 getProjectionMatrix() const;

    void zoom(float delta);
    void move(const Vector3& direction);

    Vector3 getForwardVector() const;
    Vector3 getRightVector() const;
    Vector3 getUpVector() const;

    Vector3 getPosition() const;
    void setPosition(const Vector3& newPosition);

    void rotateYaw(float deltaYaw);
    void rotatePitch(float deltaPitch);

    void orbitYaw(const Vector3& center, float deltaYaw, float deltaPitch);
    void orbit(const Vector3& center, float deltaYaw, float deltaPitch);

private:
    static Vector4 toQuaternion(const Matrix4& mat);
    Matrix4 quatToMatrix(const Vector4& q) const;
    void updateRotation();

    Vector3 position;
    Vector4 rotation;

    float windowWidth, windowHeight;
    float fov, nearClip, farClip;

    float yaw;   // Rotation around the Y-axis
    float pitch; // Rotation around the X-axis

    static constexpr float DEG_TO_RAD = 3.14159265359f / 180.0f;
    const float RAD_TO_DEG = 180.0f / 3.14159265359f;
};

#endif
