#ifndef FRUSTUM_HPP
#define FRUSTUM_HPP

#include "./math/Vector3.hpp"
#include "./math/Matrix4.hpp"
#include <array>

class Plane {
public:
    Vector3 normal;
    float distance;

    float distanceToPoint(const Vector3& point) const {
        return normal.dot(point) + distance;
    }

    void normalize() {
        float length = normal.length();
        normal = normal / length;
        distance /= length;
    }
};

class Frustum {
public:
    Frustum() = default;
    Frustum(const Matrix4& projectionMatrix, const Matrix4& viewMatrix);

    void update(const Matrix4& projectionMatrix, const Matrix4& viewMatrix);

    bool isPointInside(const Vector3& point) const;
    bool isSphereInside(const Vector3& center, float radius) const;
    bool isBoxInside(const Vector3& min, const Vector3& max) const;

private:
    std::array<Plane, 6> planes;

    void extractPlanes(const Matrix4& combinedMatrix);
    std::pair<Vector3, Vector3> getPositiveNegativeVertices(
        const Plane& plane, const Vector3& min, const Vector3& max) const;
};

#endif // FRUSTUM_HPP
