#include "Vector3.hpp"

class Plane {
public:
    Vector3 base, normal;

    // Constructor
    Plane(const Vector3& base, const Vector3& normal) : base(base), normal(normal) {}

    // Calculate signed distance from a point to the plane
    float getSignedDistanceFromPointToPlane(const Vector3& point) const {
        return normal.dot(point - base);
    }
};