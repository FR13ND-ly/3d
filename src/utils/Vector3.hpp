#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>

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
        if (scalar == 0) {
            return Vector3(0, 0, 0);
        }
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    Vector3 normalized() const {
        float length = std::sqrt(x * x + y * y + z * z);
        if (length == 0) return *this;
        return *this / length;
    }

    // Cross product of two vectors
    Vector3 cross(const Vector3& other) const {
        return Vector3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    // Dot product of two vectors
    float dot(const Vector3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
};

#endif