#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <iostream>
#include "Vector3.hpp"
#include <cmath>
#include "Matrix4.hpp"  // Ensure you include Matrix4 header for matrix operations

class Vector4 {
public:
    float x, y, z, w;

    // Constructors
    Vector4() : x(0), y(0), z(0), w(1) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vector4(const Vector3& v, float w = 1.0f) : x(v.x), y(v.y), z(v.z), w(w) {} // Convert Vector3 to Vector4

    // Operator overload for addition
    Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    // Operator overload for subtraction
    Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    // Operator overload for scalar multiplication
    Vector4 operator*(float scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    // Operator overload for matrix-vector multiplication
    friend Vector4 operator*(const Matrix4& mat, const Vector4& vec) {
        return Vector4(
            mat.data[0][0] * vec.x + mat.data[0][1] * vec.y + mat.data[0][2] * vec.z + mat.data[0][3] * vec.w,
            mat.data[1][0] * vec.x + mat.data[1][1] * vec.y + mat.data[1][2] * vec.z + mat.data[1][3] * vec.w,
            mat.data[2][0] * vec.x + mat.data[2][1] * vec.y + mat.data[2][2] * vec.z + mat.data[2][3] * vec.w,
            mat.data[3][0] * vec.x + mat.data[3][1] * vec.y + mat.data[3][2] * vec.z + mat.data[3][3] * vec.w
        );
    }

    // Length of the Vector4
    float length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    // Normalize the Vector4 (returns a unit vector)
    Vector4 normalized() const {
        float len = length();
        if (len < 1e-6f) {
            std::cerr << "Warning: Normalizing a zero-length vector!" << std::endl;
            return Vector4(0, 0, 0, 0);
        }
        return *this * (1.0f / len);
    }

    // Convert the Vector4 to Vector3 (homogeneous division by w)
    Vector3 toVector3() const {
        if (std::abs(w) > 1e-6f) {
            return Vector3(x / w, y / w, z / w);
        }
        return Vector3(x, y, z);  // When w is zero, return as a Vector3
    }

    // Print the vector (for debugging)
    friend std::ostream& operator<<(std::ostream& os, const Vector4& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
        return os;
    }

    // Equality check (use small epsilon for floating point comparison)
    bool operator==(const Vector4& other) const {
        const float epsilon = 1e-6f;
        return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon && std::abs(w - other.w) < epsilon;
    }

    // Inequality check
    bool operator!=(const Vector4& other) const {
        return !(*this == other);
    }

    // Quaternion operations
    static Vector4 eulerToQuaternion(float pitch, float yaw, float roll) {
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);

        return Vector4(
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy,
            cr * cp * cy + sr * sp * sy
        );
    }

    static Vector4 quaternionMultiply(const Vector4& q1, const Vector4& q2) {
        return Vector4(
            q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
            q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
            q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w,
            q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
        );
    }

    static Vector4 quaternionNormalize(const Vector4& q) {
        float length = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
        return Vector4(q.x / length, q.y / length, q.z / length, q.w / length);
    }

    static Vector3 quaternionToEuler(const Vector4& q) {
        float ysqr = q.y * q.y;

        float t0 = +2.0f * (q.w * q.x + q.y * q.z);
        float t1 = +1.0f - 2.0f * (q.x * q.x + ysqr);
        float X = std::atan2(t0, t1);

        float t2 = +2.0f * (q.w * q.y - q.z * q.x);
        t2 = t2 > 1.0f ? 1.0f : t2;
        t2 = t2 < -1.0f ? -1.0f : t2;
        float Y = std::asin(t2);

        float t3 = +2.0f * (q.w * q.z + q.x * q.y);
        float t4 = +1.0f - 2.0f * (ysqr + q.z * q.z);
        float Z = std::atan2(t3, t4);

        return Vector3(X, Y, Z);
    }
};

#endif // VECTOR4_HPP
