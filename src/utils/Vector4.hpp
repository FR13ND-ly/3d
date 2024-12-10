#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <iostream>
#include "Vector3.hpp"
#include <cmath>
#include "Matrix4.hpp"

class Vector4 {
public:
    float x, y, z, w;

    Vector4() : x(0), y(0), z(0), w(1) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vector4(const Vector3& v, float w = 1.0f) : x(v.x), y(v.y), z(v.z), w(w) {}

    Vector4 operator+(const Vector4& other) const {
        return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
    }

    Vector4 operator-(const Vector4& other) const {
        return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
    }

    Vector4 operator*(float scalar) const {
        return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
    }

    Vector4 operator*(const Vector4& other) const {
        return Vector4(
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w,
            w * other.w - x * other.x - y * other.y - z * other.z
        );
    }

    friend Vector4 operator*(const Matrix4& mat, const Vector4& vec) {
        return Vector4(
            mat.data[0][0] * vec.x + mat.data[0][1] * vec.y + mat.data[0][2] * vec.z + mat.data[0][3] * vec.w,
            mat.data[1][0] * vec.x + mat.data[1][1] * vec.y + mat.data[1][2] * vec.z + mat.data[1][3] * vec.w,
            mat.data[2][0] * vec.x + mat.data[2][1] * vec.y + mat.data[2][2] * vec.z + mat.data[2][3] * vec.w,
            mat.data[3][0] * vec.x + mat.data[3][1] * vec.y + mat.data[3][2] * vec.z + mat.data[3][3] * vec.w
        );
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z + w * w);
    }

    Vector4 normalized() const {
        float len = length();
        if (len < 1e-6f) {
            std::cerr << "Warning: Normalizing a zero-length vector!" << std::endl;
            return Vector4(0, 0, 0, 0);
        }
        return *this * (1.0f / len);
    }

    static Vector4 axisAngleToQuaternion(const Vector3& axis, float angle) {
        // Normalize the axis vector
        Vector3 normalizedAxis = axis.normalized();

        // Compute the components
        float halfAngle = angle * 0.5f;
        float sinHalfAngle = sin(halfAngle);
        float cosHalfAngle = cos(halfAngle);

        return Vector4(
            cosHalfAngle,                           // w
            sinHalfAngle * normalizedAxis.x,        // x
            sinHalfAngle * normalizedAxis.y,        // y
            sinHalfAngle * normalizedAxis.z         // z
        );
    }

    Vector3 toVector3() const {
        if (std::abs(w) > 1e-6f) {
            return Vector3(x / w, y / w, z / w);
        }
        return Vector3(x, y, z);
    }

    friend std::ostream& operator<<(std::ostream& os, const Vector4& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
        return os;
    }

    bool operator==(const Vector4& other) const {
        const float epsilon = 1e-6f;
        return std::abs(x - other.x) < epsilon && std::abs(y - other.y) < epsilon &&
               std::abs(z - other.z) < epsilon && std::abs(w - other.w) < epsilon;
    }

    bool operator!=(const Vector4& other) const {
        return !(*this == other);
    }

    static Vector4 eulerToQuaternion(float pitch, float yaw, float roll) {
        float cr = std::cos(roll * 0.5f);
        float sr = std::sin(roll * 0.5f);
        float cp = std::cos(pitch * 0.5f);
        float sp = std::sin(pitch * 0.5f);
        float cy = std::cos(yaw * 0.5f);
        float sy = std::sin(yaw * 0.5f);

        Vector4 quat(
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy
        );

        return quat.normalized();
    }

    static Vector4 quaternionMultiply(const Vector4& q1, const Vector4& q2) {
        return Vector4(
            q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z,
            q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
            q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x,
            q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
        );
    }

    static Vector4 quaternionNormalize(const Vector4& q) {
        return q.normalized();
    }

    static Vector3 quaternionToEuler(const Vector4& q) {
        Vector4 normalizedQ = q.normalized();
        float x = normalizedQ.x;
        float y = normalizedQ.y;
        float z = normalizedQ.z;
        float w = normalizedQ.w;

        float ysqr = y * y;

        float t0 = +2.0f * (w * x + y * z);
        float t1 = +1.0f - 2.0f * (x * x + ysqr);
        float X = std::atan2(t0, t1);

        float t2 = +2.0f * (w * y - z * x);
        t2 = t2 > 1.0f ? 1.0f : t2;
        t2 = t2 < -1.0f ? -1.0f : t2;
        float Y = std::asin(t2);

        float t3 = +2.0f * (w * z + x * y);
        float t4 = +1.0f - 2.0f * (ysqr + z * z);
        float Z = std::atan2(t3, t4);

        return Vector3(X, Y, Z);
    }

    static Vector4 rotateVectorByQuaternion(const Vector4& q, const Vector3& v) {
        Vector4 vec(v, 0.0f);
        Vector4 rotated = quaternionMultiply(quaternionMultiply(q, vec), quaternionConjugate(q));
        return rotated;
    }

    static Vector4 quaternionConjugate(const Vector4& q) {
        return Vector4(-q.x, -q.y, -q.z, q.w);
    }


};

#endif
