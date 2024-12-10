#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <iostream> // Optional, for debugging

class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    Vector3 operator/(float scalar) const {
        if (std::abs(scalar) < 1e-6f) {
            std::cerr << "Warning: Division by zero!" << std::endl;
            return Vector3(0, 0, 0);
        }
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    Vector3 normalized() const {
        float length = magnitude();
        if (length < 1e-6f) {
            std::cerr << "Warning: Normalizing a zero-length vector!" << std::endl;
            return Vector3(0, 0, 0);
        }
        return *this / length;
    }

    Vector3 toEulerAngles() const {
        Vector3 dir = this->normalized();

        float pitch = std::asin(dir.y);
        float yaw = std::atan2(-dir.x, dir.z);

        return Vector3(pitch, yaw, 0.0f);
    }

    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float length() const {
        return magnitude();
    }

    void print() const {
        std::cout << "Vector3(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    bool operator==(const Vector3& other) const {
        const float epsilon = 1e-6f;
        return std::abs(x - other.x) < epsilon &&
               std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }

    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }
};

#endif
