#include <array>
#include <cmath>
#include "Vector3.hpp"
#include "Plane.hpp"

class AABB {
public:
    Vector3 min, max;  // Minimum and maximum corners of the AABB

    // Constructor
    AABB(const Vector3& min_point, const Vector3& max_point) : min(min_point), max(max_point) {}

    // Check if the AABB is inside the frustum
    bool isInsideFrustum(const std::array<Plane, 6>& frustrum) const {
        for (const auto& plane : frustrum) {
            if (isOutsideOfPlane(plane)) {
                return false;  // Outside the frustum
            }
        }
        return true;  // Inside the frustum
    }

private:
    // Check if the AABB is outside a specific plane
    bool isOutsideOfPlane(const Plane& plane) const {
        // Get all 8 corners of the AABB
        Vector3 corners[8] = {
            Vector3(min.x, min.y, min.z), Vector3(min.x, min.y, max.z), Vector3(min.x, max.y, min.z), Vector3(min.x, max.y, max.z),
            Vector3(max.x, min.y, min.z), Vector3(max.x, min.y, max.z), Vector3(max.x, max.y, min.z), Vector3(max.x, max.y, max.z)
        };

        // Check if all corners are behind the plane
        for (const auto& corner : corners) {
            if (plane.getSignedDistanceFromPointToPlane(corner) >= 0) {
                return false;  // At least one corner is in front of the plane
            }
        }
        return true;  // All corners are behind the plane, so the AABB is outside the frustum
    }
};
