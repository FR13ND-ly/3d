#ifndef MATRIX4_HPP
#define MATRIX4_HPP

#include "Vector3.hpp"
#include <array>
#include <cmath>

class Matrix4 {
public:
    std::array<std::array<float, 4>, 4> data;

    // Constructors
    Matrix4() {
        for (auto& row : data) row.fill(0.0f);
    }

    static Matrix4 identity() {
        Matrix4 mat;
        for (int i = 0; i < 4; i++) mat.data[i][i] = 1.0f;
        return mat;
    }

    static Matrix4 translation(float x, float y, float z) {
        Matrix4 mat = identity();
        mat.data[0][3] = x;
        mat.data[1][3] = y;
        mat.data[2][3] = z;
        return mat;
    }

    static Matrix4 scale(float sx, float sy, float sz) {
        Matrix4 mat = identity();
        mat.data[0][0] = sx;
        mat.data[1][1] = sy;
        mat.data[2][2] = sz;
        return mat;
    }

    static Matrix4 rotationX(float angle) {
        Matrix4 mat = identity();
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat.data[1][1] = c;
        mat.data[1][2] = -s;
        mat.data[2][1] = s;
        mat.data[2][2] = c;
        return mat;
    }

    static Matrix4 rotationY(float angle) {
        Matrix4 mat = identity();
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat.data[0][0] = c;
        mat.data[0][2] = s;
        mat.data[2][0] = -s;
        mat.data[2][2] = c;
        return mat;
    }

    static Matrix4 rotationZ(float angle) {
        Matrix4 mat = identity();
        float c = std::cos(angle);
        float s = std::sin(angle);
        mat.data[0][0] = c;
        mat.data[0][1] = -s;
        mat.data[1][0] = s;
        mat.data[1][1] = c;
        return mat;
    }

    static Matrix4 perspective(float fov, float aspect, float near, float far) {
        Matrix4 mat;
        float tanHalfFov = std::tan(fov / 2.0f);
        mat.data[0][0] = 1.0f / (aspect * tanHalfFov);
        mat.data[1][1] = 1.0f / tanHalfFov;
        mat.data[2][2] = -(far + near) / (far - near);
        mat.data[2][3] = -(2.0f * far * near) / (far - near);
        mat.data[3][2] = -1.0f;
        return mat;
    }

    // Operators
    Matrix4 operator*(const Matrix4& other) const {
        Matrix4 result;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                result.data[i][j] = 0.0f;
                for (int k = 0; k < 4; ++k) {
                    result.data[i][j] += data[i][k] * other.data[k][j];
                }
            }
        }
        return result;
    }

    Vector3 operator*(const Vector3& vec) const {
        Vector3 result;
        result.x = data[0][0] * vec.x + data[0][1] * vec.y + data[0][2] * vec.z + data[0][3];
        result.y = data[1][0] * vec.x + data[1][1] * vec.y + data[1][2] * vec.z + data[1][3];
        result.z = data[2][0] * vec.x + data[2][1] * vec.y + data[2][2] * vec.z + data[2][3];
        float w = data[3][0] * vec.x + data[3][1] * vec.y + data[3][2] * vec.z + data[3][3];
        if (w != 0.0f) {
            result.x /= w;
            result.y /= w;
            result.z /= w;
        }
        return result;
    }
};

#endif
