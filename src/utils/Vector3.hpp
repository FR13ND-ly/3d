#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <iostream> // Optional, for debugging

class Vector3 {
public:
    float x, y, z;

    // Constructors
    Vector3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

    // Vector addition
    Vector3 operator+(const Vector3& other) const {
        return Vector3(x + other.x, y + other.y, z + other.z);
    }

    // Vector subtraction
    Vector3 operator-(const Vector3& other) const {
        return Vector3(x - other.x, y - other.y, z - other.z);
    }

    // Scalar multiplication
    Vector3 operator*(float scalar) const {
        return Vector3(x * scalar, y * scalar, z * scalar);
    }

    // Scalar division
    Vector3 operator/(float scalar) const {
        if (std::abs(scalar) < 1e-6f) { // Avoid division by zero
            std::cerr << "Warning: Division by zero!" << std::endl;
            return Vector3(0, 0, 0);
        }
        return Vector3(x / scalar, y / scalar, z / scalar);
    }

    // Normalize the vector (returns a unit vector)
    Vector3 normalized() const {
        float length = magnitude();
        if (length < 1e-6f) { // Avoid division by near-zero values
            std::cerr << "Warning: Normalizing a zero-length vector!" << std::endl;
            return Vector3(0, 0, 0);
        }
        return *this / length;
    }

    // Converts the vector to Euler angles (pitch, yaw, roll)
    Vector3 toEulerAngles() const {
        // Normalize the direction vector
        Vector3 dir = this->normalized();

        // Calculate pitch and yaw
        float pitch = std::asin(dir.y);               // Pitch (rotation around X-axis)
        float yaw = std::atan2(-dir.x, dir.z);        // Yaw (rotation around Y-axis)

        // Roll is assumed to be zero in this context
        return Vector3(pitch, yaw, 0.0f);
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

    // Magnitude of the vector
    float magnitude() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    // Length of the vector (alias for magnitude)
    float length() const {
        return magnitude();
    }

    // Prints the vector (useful for debugging)
    void print() const {
        std::cout << "Vector3(" << x << ", " << y << ", " << z << ")" << std::endl;
    }

    // Equality check (for comparing vectors)
    bool operator==(const Vector3& other) const {
        const float epsilon = 1e-6f;
        return std::abs(x - other.x) < epsilon &&
               std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon;
    }

    // Inequality check
    bool operator!=(const Vector3& other) const {
        return !(*this == other);
    }
};

#endif
