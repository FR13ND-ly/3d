#include "ObjectsFactory.hpp"
#include <cmath>

std::shared_ptr<Object3d> ObjectsFactory::createObject(const std::string& objectType, float param1, int param2) {
    if (objectType == "cube") {
        return std::make_shared<CustomShape>(createCube(param1));
    } else if (objectType == "sphere") {
        return std::make_shared<CustomShape>(createSphere(param1, param2));
    } else if (objectType == "cylinder") {
        return std::make_shared<CustomShape>(createCylinder(param1, param2));
    } else if (objectType == "cone") {
        return std::make_shared<CustomShape>(createCone(param1, param2));
    } else {
        throw std::invalid_argument("Unsupported object type: " + objectType);
    }
}

CustomShape ObjectsFactory::createCylinder(float radius, int segments) {
    std::vector<Vector3> vertices;
    std::vector<std::array<int, 7>> faces;

    // Generate vertices for bottom and top layers
    for (int i = 0; i <= 1; ++i) { // Two layers: bottom and top
        float z = i == 0 ? -radius : radius;
        for (int j = 0; j < segments; ++j) {
            float angle = 2 * M_PI * j / segments;
            vertices.push_back({
                static_cast<float>(radius * cos(angle)),
                static_cast<float>(radius * sin(angle)),
                z
            });
        }
    }

    int bottomStart = 0;               // Start index for the bottom layer
    int topStart = segments;           // Start index for the top layer

    // Add faces for the side
    for (int i = 0; i < segments; ++i) {
        int next = (i + 1) % segments;
        int bottom = bottomStart + i;
        int top = topStart + i;
        int bottomNext = bottomStart + next;
        int topNext = topStart + next;

        faces.push_back({bottom, top, topNext, 128, 128, 128, 255}); // Greyish
        faces.push_back({bottom, topNext, bottomNext, 160, 160, 160, 255}); // Slightly lighter grey
    }

    // Add caps
    int centerBottom = vertices.size();
    int centerTop = vertices.size() + 1;
    vertices.push_back({0, 0, -radius}); // Bottom center
    vertices.push_back({0, 0, radius});  // Top center
    for (int i = 0; i < segments; ++i) {
        int next = (i + 1) % segments;
        faces.push_back({centerBottom, bottomStart + i, bottomStart + next, 100, 100, 100, 255}); // Dark grey
        faces.push_back({centerTop, topStart + i, topStart + next, 200, 200, 200, 255});         // Light grey
    }

    return CustomShape(vertices, faces);
}

CustomShape ObjectsFactory::createCone(float radius, int segments) {
    std::vector<Vector3> vertices;
    std::vector<std::array<int, 7>> faces;

    for (int i = 0; i < segments; ++i) {
        float angle = 2 * M_PI * i / segments;
        vertices.push_back({
            static_cast<float>(radius * cos(angle)),
            static_cast<float>(radius * sin(angle)),
            -radius
        });
    }

    int apexIndex = vertices.size();
    vertices.push_back({0, 0, radius});

    int baseCenterIndex = vertices.size();
    vertices.push_back({0, 0, -radius});

    for (int i = 0; i < segments; ++i) {
        int next = (i + 1) % segments;

        int baseColor = i % 2 == 0 ? 100 : 168;
        faces.push_back({i, apexIndex, next, baseColor, baseColor, baseColor, 255});
        baseColor = i % 2 == 0 ? 100 : 168;
        faces.push_back({baseCenterIndex, i, next, baseColor, baseColor, baseColor, 255});
    }

    return CustomShape(vertices, faces);
}

CustomShape ObjectsFactory::createCube(float size) {
    float half = size / 2.0f;

    std::vector<Vector3> vertices = {
        Vector3(-half, -half, -half),
        Vector3(half, -half, -half),
        Vector3(half, half, -half),
        Vector3(-half, half, -half),

        Vector3(-half, -half, half),
        Vector3(half, -half, half),
        Vector3(half, half, half),
        Vector3(-half, half, half)
    };

    std::vector<std::array<int, 7>> faces = {
        {0, 1, 2, 128, 128, 128, 255}, // Grey
        {0, 2, 3, 128, 128, 128, 255}, // Grey

        {4, 6, 5, 160, 160, 160, 255}, // Lighter grey
        {4, 7, 6, 160, 160, 160, 255}, // Lighter grey

        {1, 5, 2, 100, 100, 100, 255}, // Dark grey
        {5, 6, 2, 100, 100, 100, 255}, // Dark grey

        {0, 3, 4, 200, 200, 200, 255}, // Light grey
        {3, 7, 4, 200, 200, 200, 255}, // Light grey

        {0, 4, 1, 140, 140, 140, 255}, // Medium grey
        {4, 5, 1, 140, 140, 140, 255}, // Medium grey

        {3, 2, 7, 120, 120, 120, 255}, // Medium-dark grey
        {2, 6, 7, 120, 120, 120, 255}  // Medium-dark grey
    };
    return CustomShape(vertices, faces);
}

CustomShape ObjectsFactory::createSphere(float radius, int segments) {
    std::vector<Vector3> vertices;
    std::vector<std::array<int, 7>> faces;

    // Generate vertices
    for (int i = 0; i <= segments; ++i) {
        float theta = M_PI * i / segments;
        for (int j = 0; j <= segments; ++j) { // Note: Changed to <= segments to include the last vertex
            float phi = 2 * M_PI * j / segments;
            vertices.push_back({
                static_cast<float>(radius * sin(theta) * cos(phi)),
                static_cast<float>(radius * sin(theta) * sin(phi)),
                static_cast<float>(radius * cos(theta))
            });
        }
    }

    // Calculate faces
    for (int lat = 0; lat < segments; ++lat) {
        for (int lon = 0; lon < segments; ++lon) {
            int first = lat * (segments + 1) + lon;
            int second = first + segments + 1;

            // Side faces with greyish colors
            faces.push_back({
                first, second, first + 1,
                static_cast<int>(150 + 50 * (lon % 2)),  // Alternate between shades of grey
                static_cast<int>(150 + 50 * (lon % 2)),
                static_cast<int>(150 + 50 * (lon % 2)),
                255
            });
            faces.push_back({
                second, second + 1, first + 1,
                static_cast<int>(130 + 70 * (lat % 2)),  // Alternate between shades of grey
                static_cast<int>(130 + 70 * (lat % 2)),
                static_cast<int>(130 + 70 * (lat % 2)),
                255
            });
        }
    }

    return CustomShape(vertices, faces);
}

CustomShape ObjectsFactory::createCustomShape(const std::vector<Vector3>& vertices,
                                             const std::vector<std::array<int, 7>>& faces) {
    return CustomShape(vertices, faces);
}
