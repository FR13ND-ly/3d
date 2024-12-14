#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <iostream>
#include "Vector3.hpp"
#include <cmath>
#include "Matrix4.hpp"

class Vector4 {
public:
    float x, y, z, w;

    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4(const Vector3& v, float w = 1.0f);

    Vector4 operator+(const Vector4& other) const;
    Vector4 operator-(const Vector4& other) const;
    Vector4 operator*(float scalar) const;
    Vector4 operator*(const Vector4& other) const;

    friend Vector4 operator*(const Matrix4& mat, const Vector4& vec);

    float length() const;
    Vector4 normalized() const;

    static Vector4 axisAngleToQuaternion(const Vector3& axis, float angle);
    Vector3 toVector3() const;

    friend std::ostream& operator<<(std::ostream& os, const Vector4& vec);

    bool operator==(const Vector4& other) const;
    bool operator!=(const Vector4& other) const;

    static Vector4 eulerToQuaternion(float pitch, float yaw, float roll);
    static Vector4 quaternionMultiply(const Vector4& q1, const Vector4& q2);
    static Vector4 quaternionNormalize(const Vector4& q);
    static Vector3 quaternionToEuler(const Vector4& q);
    static Vector4 rotateVectorByQuaternion(const Vector4& q, const Vector3& v);
    static Vector4 quaternionConjugate(const Vector4& q);
};

#endif // VECTOR4_HPP
