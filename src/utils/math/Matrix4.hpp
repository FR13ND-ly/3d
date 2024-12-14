#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include "Vector3.hpp"
#include <array>

class Matrix4 {
public:
    std::array<std::array<float, 4>, 4> data;

    // Constructors
    Matrix4();
    Matrix4(std::array<std::array<float, 4>, 4> initData);

    // Static methods for common transformations
    static Matrix4 identity();
    static Matrix4 translation(float x, float y, float z);
    static Matrix4 scale(float sx, float sy, float sz);
    static Matrix4 rotationX(float angle);
    static Matrix4 rotationY(float angle);
    static Matrix4 rotationZ(float angle);
    static Matrix4 perspective(float fov, float aspect, float near, float far);
    static Matrix4 orthographic(float left, float right, float bottom, float top, float near, float far);
    static Matrix4 lookAt(const Vector3& eye, const Vector3& center, const Vector3& up);

    // Operators
    Matrix4 operator*(const Matrix4& other) const;
    Vector3 operator*(const Vector3& vec) const;
};

#endif // MATRIX4_HPP
