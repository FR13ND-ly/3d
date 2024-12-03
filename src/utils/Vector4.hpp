#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include <iostream>
#include "Vector3.hpp"

class Vector4 {
public:
    float x, y, z, w;

    // Constructors
    Vector4() : x(0), y(0), z(0), w(1) {}
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    Vector4(const Vector3 &v, float w = 1.0f) : x(v.x), y(v.y), z(v.z), w(w) {} // Convert Vector3 to Vector4

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

    // Print the vector (for debugging)
    friend std::ostream& operator<<(std::ostream& os, const Vector4& vec) {
        os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w << ")";
        return os;
    }
};

#endif // VECTOR4_HPP
