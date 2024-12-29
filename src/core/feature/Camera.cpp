#include "Camera.hpp"
#include <cmath>
#include <array>
#include <algorithm>

Camera::Camera(float windowWidth, float windowHeight, float fov, float nearClip, float farClip)
    : position(-1.5f, 2.0f, -15.0f),
      rotation(Vector4::eulerToQuaternion(0, 0, 0)),
      orbitCenter(0.0f, 0.0f, 0.0f),
      windowWidth(windowWidth),
      windowHeight(windowHeight),
      fov(fov),
      nearClip(nearClip),
      farClip(farClip),
      yaw(0.0f),
      pitch(0.0f) {
    rotatePitch(-5.f);
    rotateYaw(-2.f);
}

Matrix4 Camera::getViewMatrix() const {
    Matrix4 rotationMatrix = quatToMatrix(Vector4::quaternionConjugate(rotation));
    Matrix4 translationMatrix = Matrix4::translation(-position.x, -position.y, -position.z);
    Matrix4 flipZ = Matrix4::scale(1.0f, 1.0f, -1.0f);

    return flipZ * rotationMatrix * translationMatrix;
}

Matrix4 Camera::getProjectionMatrix() const {
    float aspectRatio = windowWidth / windowHeight;
    float fovRadians = fov * DEG_TO_RAD;
    return Matrix4::perspective(fovRadians, aspectRatio, nearClip, farClip);
}

void Camera::zoom(float delta) {
    Vector3 target(0, 0, 0);
    Vector3 toTarget = (target - position).normalized();
    Vector3 forward = getForwardVector();
    float alignment = toTarget.dot(forward);
    float zoomDirection = (alignment > 0) ? 1.0f : -1.0f;
    position = position + (forward * delta * zoomDirection);
}

void Camera::move(const Vector3& direction) {
    Vector3 forward = getForwardVector();
    Vector3 right = getRightVector();
    Vector3 up = getUpVector();

    position = position + (forward * direction.z) + (right * direction.x) + (up * direction.y);
    Vector3 deltaMove = (forward * direction.z) + (right * direction.x) + (up * direction.y);
    orbitCenter = orbitCenter + deltaMove;
}

Vector3 Camera::getForwardVector() const {
    float radYaw = yaw * DEG_TO_RAD;
    float radPitch = pitch * DEG_TO_RAD;

    float x = std::cos(radPitch) * std::sin(radYaw);
    float y = -std::sin(radPitch);
    float z = -std::cos(radPitch) * std::cos(radYaw);

    return Vector3(x, y, z).normalized();
}

Vector3 Camera::getRightVector() const {
    Vector3 forward = getForwardVector();
    Vector3 worldUp(0.0f, 1.0f, 0.0f);

    Vector3 right = forward.cross(worldUp).normalized();
    return right;
}

Vector3 Camera::getUpVector() const {
    Vector3 forward = getForwardVector();
    Vector3 right = getRightVector();

    Vector3 up = right.cross(forward).normalized();
    return up;
}

Vector3 Camera::getPosition() const {
    return position;
}

void Camera::setPosition(const Vector3& newPosition) {
    position = newPosition;
}

void Camera::rotate(float deltaYaw, float deltaPitch) {
    yaw += deltaYaw;
    pitch += deltaPitch;
    pitch = std::clamp(pitch, -89.0f, 89.0f);
    updateRotation();
}

void Camera::rotateYaw(float deltaYaw) {
    yaw += deltaYaw;
    updateRotation();
}

void Camera::rotatePitch(float deltaPitch) {
    pitch += deltaPitch;
    pitch = std::clamp(pitch, -89.0f, 89.0f);
    updateRotation();
}

void Camera::orbitYaw(const Vector3& center, float deltaYaw, float deltaPitch) {
    Vector3 positionXZ(position.x, 0.0f, position.z);
    float radius = positionXZ.length();

    float currentAngle = std::atan2(position.z - center.z, position.x - center.x);

    float newAngle = currentAngle + deltaYaw * DEG_TO_RAD;

    float newX = center.x + radius * std::cos(newAngle);
    float newZ = center.z + radius * std::sin(newAngle);

    position.x = newX;
    position.z = newZ;

    rotateYaw(deltaYaw);
}

void Camera::orbit(float deltaYaw, float deltaPitch) {
    Vector3 positionXZ(position.x, 0.0f, position.z);
    float radius = (positionXZ - Vector3(orbitCenter.x, 0.0f, orbitCenter.z)).length();

    float currentYaw = std::atan2(position.z - orbitCenter.z, position.x - orbitCenter.x);
    float newYaw = currentYaw + deltaYaw * DEG_TO_RAD;

    float newX = orbitCenter.x + radius * std::cos(newYaw);
    float newZ = orbitCenter.z + radius * std::sin(newYaw);

    float currentPitch = std::atan2(position.y - orbitCenter.y, radius);
    float newPitch = currentPitch + deltaPitch * DEG_TO_RAD;

    float newY = orbitCenter.y + radius * std::tan(newPitch);

    position.x = newX;
    position.z = newZ;

    rotateYaw(deltaYaw);

    if (newPitch > -0.9 && newPitch < 0.9) {
        position.y = newY;
        rotatePitch(-deltaPitch);
    }
}

float Camera::getYaw() const {
    return yaw;
}

float Camera::getPitch() const {
    return pitch;
}

void Camera::setYawAndPitch(float yaw, float pitch) {
    this->yaw = yaw;
    this->pitch = pitch;
    updateRotation();
}

Vector3 Camera::getOrbitCenter() const {
    return orbitCenter;
}

void Camera::setOrbitCenter(const Vector3& center) {
    orbitCenter = center;
}

Vector4 Camera::toQuaternion(const Matrix4& mat) {
    float trace = mat.data[0][0] + mat.data[1][1] + mat.data[2][2];
    float s;

    if (trace > 0) {
        s = 0.5f / std::sqrt(trace + 1.0f);
        return Vector4(
            (mat.data[2][1] - mat.data[1][2]) * s,
            (mat.data[0][2] - mat.data[2][0]) * s,
            (mat.data[1][0] - mat.data[0][1]) * s,
            0.25f / s
        );
    } else {
        if (mat.data[0][0] > mat.data[1][1] && mat.data[0][0] > mat.data[2][2]) {
            s = 2.0f * std::sqrt(1.0f + mat.data[0][0] - mat.data[1][1] - mat.data[2][2]);
            return Vector4(
                0.25f * s,
                (mat.data[0][1] + mat.data[1][0]) / s,
                (mat.data[0][2] + mat.data[2][0]) / s,
                (mat.data[2][1] - mat.data[1][2]) / s
            );
        } else if (mat.data[1][1] > mat.data[2][2]) {
            s = 2.0f * std::sqrt(1.0f + mat.data[1][1] - mat.data[0][0] - mat.data[2][2]);
            return Vector4(
                (mat.data[0][1] + mat.data[1][0]) / s,
                0.25f * s,
                (mat.data[1][2] + mat.data[2][1]) / s,
                (mat.data[0][2] - mat.data[2][0]) / s
            );
        } else {
            s = 2.0f * std::sqrt(1.0f + mat.data[2][2] - mat.data[0][0] - mat.data[1][1]);
            return Vector4(
                (mat.data[0][2] + mat.data[2][0]) / s,
                (mat.data[1][2] + mat.data[2][1]) / s,
                0.25f * s,
                (mat.data[1][0] - mat.data[0][1]) / s
            );
        }
    }
}

Matrix4 Camera::quatToMatrix(const Vector4& q) const {
    float x = q.x, y = q.y, z = q.z, w = q.w;
    float x2 = x * x, y2 = y * y, z2 = z * z;
    float xy = x * y, xz = x * z, yz = y * z;
    float wx = w * x, wy = w * y, wz = w * z;

    Matrix4 mat;
    mat.data[0][0] = 1.0f - 2.0f * (y2 + z2);
    mat.data[0][1] = 2.0f * (xy - wz);
    mat.data[0][2] = 2.0f * (xz + wy);
    mat.data[0][3] = 0.0f;

    mat.data[1][0] = 2.0f * (xy + wz);
    mat.data[1][1] = 1.0f - 2.0f * (x2 + z2);
    mat.data[1][2] = 2.0f * (yz - wx);
    mat.data[1][3] = 0.0f;

    mat.data[2][0] = 2.0f * (xz - wy);
    mat.data[2][1] = 2.0f * (yz + wx);
    mat.data[2][2] = 1.0f - 2.0f * (x2 + y2);
    mat.data[2][3] = 0.0f;

    mat.data[3][0] = 0.0f;
    mat.data[3][1] = 0.0f;
    mat.data[3][2] = 0.0f;
    mat.data[3][3] = 1.0f;

    return mat;
}

void Camera::updateRotation() {
    yaw = fmod(yaw, 360.0f);
    if (yaw < 0) yaw += 360.0f;

    pitch = std::clamp(pitch, -89.0f, 89.0f);

    Vector4 yawQuat = Vector4::axisAngleToQuaternion(Vector3(0, 1, 0), yaw * DEG_TO_RAD);
    Vector4 pitchQuat = Vector4::axisAngleToQuaternion(Vector3(1, 0, 0), pitch * DEG_TO_RAD);

    rotation = Vector4::quaternionMultiply(pitchQuat, yawQuat);
    rotation = rotation.normalized();
}
