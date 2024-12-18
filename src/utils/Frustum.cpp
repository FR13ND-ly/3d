#include "Frustum.hpp"
#include <algorithm>

Frustum::Frustum(const Matrix4& projectionMatrix, const Matrix4& viewMatrix) {
    update(projectionMatrix, viewMatrix);
}

void Frustum::update(const Matrix4& projectionMatrix, const Matrix4& viewMatrix) {
    Matrix4 combinedMatrix = projectionMatrix * viewMatrix;
    extractPlanes(combinedMatrix);
}

bool Frustum::isPointInside(const Vector3& point) const {
    return std::all_of(planes.begin(), planes.end(), [&point](const Plane& plane) {
        return plane.distanceToPoint(point) >= 0;
    });
}

bool Frustum::isSphereInside(const Vector3& center, float radius) const {
    return std::all_of(planes.begin(), planes.end(), [&center, radius](const Plane& plane) {
        return plane.distanceToPoint(center) >= -radius;
    });
}

bool Frustum::isBoxInside(const Vector3& min, const Vector3& max) const {
    return std::all_of(planes.begin(), planes.end(), [this, &min, &max](const Plane& plane) {
        auto [positiveVertex, _] = getPositiveNegativeVertices(plane, min, max);
        return plane.distanceToPoint(positiveVertex) >= 0;
    });
}

void Frustum::extractPlanes(const Matrix4& combinedMatrix) {
    // Define plane extraction using offsets
    static const std::array<std::pair<int, int>, 6> indices = {
        std::make_pair(0, 0), std::make_pair(0, 1), std::make_pair(1, 0),
        std::make_pair(1, 1), std::make_pair(2, 0), std::make_pair(2, 1)
    };

    for (size_t i = 0; i < planes.size(); ++i) {
        const auto [row, sign] = indices[i];
        planes[i].normal.x = combinedMatrix(0, 3) + sign * combinedMatrix(0, row);
        planes[i].normal.y = combinedMatrix(1, 3) + sign * combinedMatrix(1, row);
        planes[i].normal.z = combinedMatrix(2, 3) + sign * combinedMatrix(2, row);
        planes[i].distance = combinedMatrix(3, 3) + sign * combinedMatrix(3, row);

        planes[i].normalize();
    }
}

std::pair<Vector3, Vector3> Frustum::getPositiveNegativeVertices(
    const Plane& plane, const Vector3& min, const Vector3& max) const {
    Vector3 positiveVertex = min, negativeVertex = max;

    if (plane.normal.x >= 0) {
        positiveVertex.x = max.x;
        negativeVertex.x = min.x;
    }
    if (plane.normal.y >= 0) {
        positiveVertex.y = max.y;
        negativeVertex.y = min.y;
    }
    if (plane.normal.z >= 0) {
        positiveVertex.z = max.z;
        negativeVertex.z = min.z;
    }

    return {positiveVertex, negativeVertex};
}
