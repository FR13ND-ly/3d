#include "ObjectsFactory.hpp"
#include <cmath>
#include <sstream>
#include "../../utils/files/ObjFile.hpp"
#include "../../utils/files/FileManager.hpp"
#include <unordered_map>
#include <unordered_set>

struct Triangle {
    Vector3 v1, v2, v3;
    std::array<int, 4> color;

    bool operator==(const Triangle& other) const {
        return (v1 == other.v1 && v2 == other.v2 && v3 == other.v3) ||
               (v1 == other.v2 && v2 == other.v3 && v3 == other.v1) ||
               (v1 == other.v3 && v2 == other.v1 && v3 == other.v2);
    }

    Vector3 getNormal() const;
};

struct TriangleHash {
    std::size_t operator()(const Triangle& triangle) const {
        std::hash<float> floatHash;
        return floatHash(triangle.v1.x) ^ floatHash(triangle.v1.y) ^ floatHash(triangle.v1.z) ^
               floatHash(triangle.v2.x) ^ floatHash(triangle.v2.y) ^ floatHash(triangle.v2.z) ^
               floatHash(triangle.v3.x) ^ floatHash(triangle.v3.y) ^ floatHash(triangle.v3.z);
    }
};

namespace {
    std::vector<Triangle> getTriangles(std::shared_ptr<Object3d> obj) {
        std::vector<Triangle> triangles;
        auto vertices = obj->getVertices();
        auto faces = obj->getFaces();
        auto modelMatrix = obj->getTransformation();

        std::vector<Vector3> worldVertices(vertices.size());
        for (size_t i = 0; i < vertices.size(); ++i) {
            worldVertices[i] = modelMatrix * vertices[i]; // Apply transformation
        }

        // Create triangles from world-space vertices
        for (const auto& face : faces) {
            if (face.size() >= 7) { // Ensure face has enough data
                triangles.push_back({
                    worldVertices[face[0]],
                    worldVertices[face[1]],
                    worldVertices[face[2]],
                    {face[3], face[4], face[5], face[6]}
                });
            }
        }
        return triangles;
    }

    std::shared_ptr<Object3d> trianglesToObject(std::vector<Triangle>& triangles) {
        std::vector<Vector3> vertices;
        std::vector<std::array<int, 7>> faces;
        std::unordered_map<std::string, int> vertexMap;

        auto getVertexIndex = [&](const Vector3& v) {
            std::string key = std::to_string(v.x) + "," +
                            std::to_string(v.y) + "," +
                            std::to_string(v.z);

            if (vertexMap.find(key) == vertexMap.end()) {
                vertexMap[key] = vertices.size();
                vertices.push_back(v);
            }
            return vertexMap[key];
        };

        for (const auto& triangle : triangles) {
            int v1 = getVertexIndex(triangle.v1);
            int v2 = getVertexIndex(triangle.v2);
            int v3 = getVertexIndex(triangle.v3);

            faces.push_back({v1, v2, v3,
                           triangle.color[0],
                           triangle.color[1],
                           triangle.color[2],
                           triangle.color[3]});
        }
        return std::make_shared<CustomShape>(vertices, faces);
    }
}

std::shared_ptr<Object3d> ObjectsFactory::combineObjects(const std::vector<std::shared_ptr<Object3d>>& objects) {
    std::vector<Triangle> result;

    for (const auto& obj : objects) {
        auto triangles = getTriangles(obj);
        result.insert(result.end(), triangles.begin(), triangles.end());
    }

    Vector3 centroid(0.0f, 0.0f, 0.0f);
    float totalArea = 0.0f;

    for (const auto& triangle : result) {
        Vector3 triangleCentroid = (triangle.v1 + triangle.v2 + triangle.v3) / 3.0f;
        float triangleArea = 0.5f * ((triangle.v2 - triangle.v1).cross(triangle.v3 - triangle.v1)).length();

        centroid = centroid + (triangleCentroid * triangleArea);
        totalArea += triangleArea;
    }

    if (totalArea > 0.0f) {
        centroid = centroid / totalArea;
    }

    auto mergedObject = trianglesToObject(result);

    auto vertices = mergedObject->getVertices();
    for (auto& vertex : vertices) {
        vertex = vertex - centroid;
    }
    mergedObject->setVertices(vertices);

    mergedObject->setPosition(centroid);

    return mergedObject;
}


std::shared_ptr<Object3d> ObjectsFactory::createObject(const std::string& objectType, float param1, int param2) {
    if (objectType == "cube") {
        return std::make_shared<CustomShape>(createCube(param1));
    } else if (objectType == "sphere") {
        return std::make_shared<CustomShape>(createSphere(param1, param2));
    } else if (objectType == "cylinder") {
        return std::make_shared<CustomShape>(createCylinder(param1, param2));
    } else if (objectType == "cone") {
        return std::make_shared<CustomShape>(createCone(param1, param2));
    } else if (objectType == "triangle") {
        return std::make_shared<CustomShape>(createTriangle());
    } else if (objectType == "torus") {
        return std::make_shared<CustomShape>(createTorus());
    } else if (objectType == "import") {
        return std::make_shared<CustomShape>(import());
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

    int bottomStart = 0;
    int topStart = segments;

    for (int i = 0; i < segments; ++i) {
        int next = (i + 1) % segments;
        int bottom = bottomStart + i;
        int top = topStart + i;
        int bottomNext = bottomStart + next;
        int topNext = topStart + next;

        faces.push_back({bottom, top, topNext, 128, 128, 128, 255});
        faces.push_back({bottom, topNext, bottomNext, 160, 160, 160, 255});
    }

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

CustomShape ObjectsFactory::createTriangle() {
    // Predefined vertices for the triangle
    Vector3 v1(0.0f, 1.0f, 0.0f);
    Vector3 v2(-1.0f, -1.0f, 0.0f);
    Vector3 v3(1.0f, -1.0f, 0.0f);

    // Faces definition with red color
    std::vector<Vector3> vertices = {v1, v2, v3};
    std::vector<std::array<int, 7>> faces = {
        {0, 1, 2, 100, 100, 100, 255} // Red color
    };

    return CustomShape(vertices, faces);
}


CustomShape ObjectsFactory::createTorus() {
    // Default parameters for the torus
    float majorRadius = 1.0f;
    float minorRadius = 0.3f;
    int majorSegments = 16;
    int minorSegments = 8;

    std::vector<Vector3> vertices;
    std::vector<std::array<int, 7>> faces;

    // Generate vertices for the torus
    for (int i = 0; i < majorSegments; ++i) {
        for (int j = 0; j < minorSegments; ++j) {
            float theta = 2.0f * M_PI * i / majorSegments;
            float phi = 2.0f * M_PI * j / minorSegments;

            float x = (majorRadius + minorRadius * cos(phi)) * cos(theta);
            float y = (majorRadius + minorRadius * cos(phi)) * sin(theta);
            float z = minorRadius * sin(phi);

            vertices.push_back(Vector3(x, y, z));
        }
    }

    // Generate faces for the torus
    for (int i = 0; i < majorSegments; ++i) {
        for (int j = 0; j < minorSegments; ++j) {
            int v0 = i * minorSegments + j;
            int v1 = i * minorSegments + (j + 1) % minorSegments;
            int v2 = ((i + 1) % majorSegments) * minorSegments + j;
            int v3 = ((i + 1) % majorSegments) * minorSegments + (j + 1) % minorSegments;

            // Generate a greyish pattern
            int patternIndex = (i + j) % 4; // Cycles through 4 different greys
            int grey;
            switch (patternIndex) {
                case 0: grey = 100; break; // Dark grey
                case 1: grey = 150; break; // Medium grey
                case 2: grey = 200; break; // Light grey
                case 3: grey = 255; break; // White
            }
            int alpha = 255; // Full opacity

            faces.push_back({v0, v1, v2, grey, grey, grey, alpha});
            faces.push_back({v2, v1, v3, grey, grey, grey, alpha});
        }
    }

    return CustomShape(vertices, faces);
}




CustomShape ObjectsFactory::createCustomShape(const std::vector<Vector3>& vertices,
                                             const std::vector<std::array<int, 7>>& faces) {
    return CustomShape(vertices, faces);
}


CustomShape ObjectsFactory::import() {
    FileManager& fileManager = FileManager::getInstance();
    std::string objFilePath = fileManager.selectFile();

    ObjFile objFile;
    objFile.read(objFilePath);

    std::vector<Vector3> vertices;
    for (const auto& vertexLine : objFile.getVertices()) {
        std::istringstream stream(vertexLine);
        std::string prefix;
        float x, y, z;

        stream >> prefix >> x >> y >> z;
        vertices.emplace_back(Vector3(x, y, z));
    }

    std::vector<std::array<int, 7>> faces;
    const auto& objFaces = objFile.getFaces();

    for (const auto& faceLine : objFaces) {
        std::istringstream stream(faceLine);
        std::string prefix;
        int v1, v2, v3, r = 128, g = 128, b = 255, a = 255;

        stream >> prefix >> v1 >> v2 >> v3;

        if (stream >> r >> g >> b >> a) {
            faces.push_back({v1 - 1, v2 - 1, v3 - 1, 128, 128, 255, 0});
        } else {
            faces.push_back({v1 - 1, v2 - 1, v3 - 1, 128, 128, 255, 0});
        }
    }

    return createCustomShape(vertices, faces);
}