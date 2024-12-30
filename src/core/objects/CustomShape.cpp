#include "CustomShape.hpp"
#include <algorithm>
#include <utility>
#include <map>

CustomShape::CustomShape(const std::vector<Vector3>& vertices,
                         const std::vector<std::array<int, 7>>& faces) {
    setVertices(vertices);
    setFaces(faces);
    calculateEdges();
}

void CustomShape::setVertices(const std::vector<Vector3>& vertices) {
    this->vertices.clear();
    this->vertices = vertices;
    calculateEdges();
}

void CustomShape::setFaces(const std::vector<std::array<int, 7>>& faces) {
    this->faces.clear();
    this->faces = faces;
    calculateEdges();
}


Vector3 calculateNormal(const std::vector<Vector3>& vertices, const std::array<int, 7>& face) {
    if (face.size() < 3) {
        return Vector3(0, 0, 0);
    }

    Vector3 v0 = vertices[face[0]];
    Vector3 v1 = vertices[face[1]];
    Vector3 v2 = vertices[face[2]];

    Vector3 edge1 = v1 - v0;
    Vector3 edge2 = v2 - v0;

    Vector3 normal = edge1.cross(edge2);

    if (normal.length() == 0) {
        return Vector3(0, 0, 0);
    }

    return normal.normalized();
}

bool areNormalsParallel(const Vector3& n1, const Vector3& n2) {
    return std::abs(n1.dot(n2)) > 0.999f; // Threshold for floating-point precision
}

bool areFacesConnected(const std::array<int, 7>& f1, const std::array<int, 7>& f2) {
    int sharedVertices = 0;
    for (int v1 : f1) {
        for (int v2 : f2) {
            if (v1 == v2) {
                sharedVertices++;
            }
        }
    }
    return sharedVertices >= 2;
}

void CustomShape::calculateEdges() {
    edges.clear();
    std::vector<std::pair<int, int>> faceEdges;
    std::vector<bool> faceVisited(faces.size(), false);

    for (size_t i = 0; i < faces.size(); ++i) {
        if (faceVisited[i]) continue;

        std::vector<size_t> compoundFaces = {i};
        faceVisited[i] = true;

        Vector3 normalI = calculateNormal(vertices, faces[i]);
        if (normalI.length() == 0) {
            // Handle the case where the normal is zero-length
            continue;
        }

        for (size_t j = 0; j < faces.size(); ++j) {
            if (i == j || faceVisited[j]) continue;

            Vector3 normalJ = calculateNormal(vertices, faces[j]);
            if (normalJ.length() == 0) {
                continue;
            }

            if (areNormalsParallel(normalI, normalJ) && areFacesConnected(faces[i], faces[j])) {
                compoundFaces.push_back(j);
                faceVisited[j] = true;
            }
        }

        faceEdges.clear();
        for (size_t faceIdx : compoundFaces) {
            for (size_t k = 0; k < 3; ++k) {
                int v1 = faces[faceIdx][k];
                int v2 = faces[faceIdx][(k + 1) % 3];
                if (v1 > v2) std::swap(v1, v2);
                faceEdges.push_back(std::make_pair(v1, v2));
            }
        }

        std::map<std::pair<int, int>, int> edgeCount;
        for (const auto& edge : faceEdges) {
            edgeCount[edge]++;
        }

        for (const auto& [edge, count] : edgeCount) {
            if (count == 1) {
                edges.push_back(edge);
            }
        }
    }
}


