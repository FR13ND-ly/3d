#include "Vector3.hpp"
#include <iostream> // Optional, for debugging

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3 Vector3::operator+(const Vector3& other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
}

Vector3 Vector3::operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
}

Vector3 Vector3::operator/(float scalar) const {
    if (std::abs(scalar) < 1e-6f) {
        std::cerr << "Warning: Division by zero!" << std::endl;
        return Vector3(0, 0, 0);
    }
    return Vector3(x / scalar, y / scalar, z / scalar);
}

Vector3 Vector3::normalized() const {
    float length = magnitude();
    if (length < 1e-6f) {
        std::cerr << "Warning: Normalizing a zero-length vector!" << std::endl;
        return Vector3(0, 0, 0);
    }
    return *this / length;
}

Vector3 Vector3::toEulerAngles() const {
    Vector3 dir = this->normalized();

    float pitch = std::asin(dir.y);
    float yaw = std::atan2(-dir.x, dir.z);

    return Vector3(pitch, yaw, 0.0f);
}

Vector3 Vector3::cross(const Vector3& other) const {
    return Vector3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

float Vector3::dot(const Vector3& other) const {
    return x * other.x + y * other.y + z * other.z;
}

float Vector3::magnitude() const {
    return std::sqrt(x * x + y * y + z * z);
}

float Vector3::length() const {
    return magnitude();
}

void Vector3::print() const {
    std::cout << "Vector3(" << x << ", " << y << ", " << z << ")" << std::endl;
}

bool Vector3::operator==(const Vector3& other) const {
    const float epsilon = 1e-6f;
    return std::abs(x - other.x) < epsilon &&
           std::abs(y - other.y) < epsilon &&
           std::abs(z - other.z) < epsilon;
}

bool Vector3::operator!=(const Vector3& other) const {
    return !(*this == other);
}
