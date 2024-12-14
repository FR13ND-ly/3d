#include "Matrix4.hpp"
#include <cmath>
#include <algorithm> // For std::clamp

Matrix4::Matrix4() {
    for (auto& row : data) row.fill(0.0f);
}

Matrix4::Matrix4(std::array<std::array<float, 4>, 4> initData) {
    data = initData;
}

Matrix4 Matrix4::identity() {
    Matrix4 mat;
    for (int i = 0; i < 4; i++) mat.data[i][i] = 1.0f;
    return mat;
}

Matrix4 Matrix4::translation(float x, float y, float z) {
    Matrix4 mat = identity();
    mat.data[0][3] = x;
    mat.data[1][3] = y;
    mat.data[2][3] = z;
    return mat;
}

Matrix4 Matrix4::scale(float sx, float sy, float sz) {
    Matrix4 mat = identity();
    mat.data[0][0] = sx;
    mat.data[1][1] = sy;
    mat.data[2][2] = sz;
    return mat;
}

Matrix4 Matrix4::rotationX(float angle) {
    Matrix4 mat = identity();
    float c = std::cos(angle);
    float s = std::sin(angle);
    mat.data[1][1] = c;
    mat.data[1][2] = -s;
    mat.data[2][1] = s;
    mat.data[2][2] = c;
    return mat;
}

Matrix4 Matrix4::rotationY(float angle) {
    Matrix4 mat = identity();
    float c = std::cos(angle);
    float s = std::sin(angle);
    mat.data[0][0] = c;
    mat.data[0][2] = s;
    mat.data[2][0] = -s;
    mat.data[2][2] = c;
    return mat;
}

Matrix4 Matrix4::rotationZ(float angle) {
    Matrix4 mat = identity();
    float c = std::cos(angle);
    float s = std::sin(angle);
    mat.data[0][0] = c;
    mat.data[0][1] = -s;
    mat.data[1][0] = s;
    mat.data[1][1] = c;
    return mat;
}

Matrix4 Matrix4::perspective(float fov, float aspect, float nearClip, float farClip) {
    float tanHalfFov = std::tan(fov / 2.0f);
    return Matrix4({
        1.0f / (aspect * tanHalfFov), 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f / tanHalfFov, 0.0f, 0.0f,
        0.0f, 0.0f, -(farClip + nearClip) / (farClip - nearClip), -1.0f,
        0.0f, 0.0f, -(2.0f * farClip * nearClip) / (farClip - nearClip), 0.0f
    });
}

Matrix4 Matrix4::orthographic(float left, float right, float bottom, float top, float nearClip, float farClip) {
    return Matrix4({
        2.0f / (right - left), 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / (farClip - nearClip), 0.0f,
        -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farClip + nearClip) / (farClip - nearClip), 1.0f
    });
}

Matrix4 Matrix4::lookAt(const Vector3& eye, const Vector3& center, const Vector3& up) {
    Vector3 forward = (eye - center).normalized(); // Corrected for RHS
    Vector3 right = forward.cross(up).normalized();
    Vector3 newUp = right.cross(forward);

    // Construct the view matrix
    Matrix4 viewMatrix = identity();

    viewMatrix.data[0][0] = right.x;
    viewMatrix.data[1][0] = right.y;
    viewMatrix.data[2][0] = right.z;
    viewMatrix.data[3][0] = -right.dot(eye);

    viewMatrix.data[0][1] = newUp.x;
    viewMatrix.data[1][1] = newUp.y;
    viewMatrix.data[2][1] = newUp.z;
    viewMatrix.data[3][1] = -newUp.dot(eye);

    viewMatrix.data[0][2] = -forward.x;
    viewMatrix.data[1][2] = -forward.y;
    viewMatrix.data[2][2] = -forward.z;
    viewMatrix.data[3][2] = forward.dot(eye);

    return viewMatrix;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const {
    Matrix4 result;

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result.data[i][j] = 0.0f;

            for (int k = 0; k < 4; ++k) {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }

            const float max_val = 1e6f;
            const float min_val = -1e6f;

            result.data[i][j] = std::clamp(result.data[i][j], min_val, max_val);
        }
    }

    return result;
}

Vector3 Matrix4::operator*(const Vector3& vec) const {
    Vector3 result;
    result.x = data[0][0] * vec.x + data[0][1] * vec.y + data[0][2] * vec.z + data[0][3];
    result.y = data[1][0] * vec.x + data[1][1] * vec.y + data[1][2] * vec.z + data[1][3];
    result.z = data[2][0] * vec.x + data[2][1] * vec.y + data[2][2] * vec.z + data[2][3];
    float w = data[3][0] * vec.x + data[3][1] * vec.y + data[3][2] * vec.z + data[3][3];

    const float epsilon = 1e-6f;
    if (std::abs(w) < epsilon) {
        result.x = result.y = result.z = 0.1f;
    } else {
        result.x /= w;
        result.y /= w;
        result.z /= w;
    }

    result.x = std::clamp(result.x, -100.0f, 100.0f);
    result.y = std::clamp(result.y, -100.0f, 100.0f);
    result.z = std::clamp(result.z, -100.0f, 100.0f);

    return result;
}
