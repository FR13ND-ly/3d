#include "Object3d.hpp"
#include "sstream"
#include <algorithm>
#include <iomanip>
#include <unordered_set>

Object3d::Object3d()
    : transform(Matrix4::identity()), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}

std::vector<std::array<int, 7>> Object3d::getSortedFaces(const std::vector<Vector3>& transformedVertices) const {
    std::vector<std::tuple<float, std::array<int, 7>>> faceDepths;

    for (const auto& face : faces) {
        float zAvg = (
            transformedVertices[face[0]].z +
            transformedVertices[face[1]].z +
            transformedVertices[face[2]].z
        ) / 3.0f;

    faceDepths.emplace_back(zAvg, face);
    }

    std::sort(faceDepths.begin(), faceDepths.end(), [](const auto& a, const auto& b) {
        return std::get<0>(a) > std::get<0>(b);
    });

    std::vector<std::array<int, 7>> sortedFaces;
    for (const auto& [_, face] : faceDepths) {
        sortedFaces.push_back(face);
    }

    return sortedFaces;
}

void Object3d::setTransform(const Matrix4& newTransform) {
    transform = newTransform;
}

void Object3d::rotate(float angle, char axis) {
    Matrix4 rotationMatrix;

    switch (axis) {
        case 'x':
            rotationMatrix = Matrix4::rotationX(angle);
        rotation.x += angle;
        break;
        case 'y':
            rotationMatrix = Matrix4::rotationY(angle);
        rotation.y += angle;
        break;
        case 'z':
            rotationMatrix = Matrix4::rotationZ(angle);
        rotation.z += angle;
        break;
        default:
            return;
    }

    transform = transform * rotationMatrix;
}

void Object3d::setRotation(const Vector3& newRotation) {
    rotation = newRotation;

    Matrix4 rotationX = Matrix4::rotationX(newRotation.x);
    Matrix4 rotationY = Matrix4::rotationY(newRotation.y);
    Matrix4 rotationZ = Matrix4::rotationZ(newRotation.z);

    Matrix4 rotationMatrix = rotationZ * rotationY * rotationX; // Assuming ZYX rotation order

    Matrix4 translationMatrix = Matrix4::translation(position.x, position.y, position.z);
    Matrix4 scaleMatrix = Matrix4::scale(scale.x, scale.y, scale.z);
    transform = translationMatrix * rotationMatrix * scaleMatrix;
}

void Object3d::translate(const Vector3& translation) {
    position = position + translation;

    Matrix4 rotationMatrix = Matrix4::rotationZ(rotation.z) *
                             Matrix4::rotationY(rotation.y) *
                             Matrix4::rotationX(rotation.x);
    Matrix4 scaleMatrix = Matrix4::scale(scale.x, scale.y, scale.z);
    Matrix4 translationMatrix = Matrix4::translation(position.x, position.y, position.z);

    transform = translationMatrix * rotationMatrix * scaleMatrix;
}

void Object3d::setScale(const Vector3& newScale) {
    scale = scale + newScale;

    Matrix4 rotationMatrix = Matrix4::rotationZ(rotation.z) *
                             Matrix4::rotationY(rotation.y) *
                             Matrix4::rotationX(rotation.x);
    Matrix4 translationMatrix = Matrix4::translation(position.x, position.y, position.z);
    Matrix4 scaleMatrix = Matrix4::scale(scale.x, scale.y, scale.z);

    transform = translationMatrix * rotationMatrix * scaleMatrix;
}

Vector3 Object3d::getPosition() const {
    return position;
}

Vector3 Object3d::getRotation() const {
    return rotation;
}

Vector3 Object3d::getScale() const {
    return scale;
}

Matrix4 Object3d::getTransformation() const {
    return transform;
}

std::vector<Vector3> Object3d::getVertices() {
    return vertices;
}

std::vector<std::pair<int, int>> Object3d::getEdges() {
    return edges;
}

std::vector<std::array<int, 7>> Object3d::getFaces() {
    return faces;
}

void Object3d::setFaceColor(int faceIndex, const std::string& hexColor) {
    if (faceIndex < 0 || faceIndex >= faces.size()) {
        // throw std::out_of_range("Invalid face index");
        return;
    }

    if (hexColor.length() != 7 && hexColor.length() != 9 || hexColor[0] != '#') {
        return;
        // throw std::invalid_argument("Invalid hex color string");
    }

    int r, g, b, a = 255;

    std::stringstream ss;
    ss << std::hex << hexColor.substr(1, 2); ss >> r;
    ss.clear();
    ss << std::hex << hexColor.substr(3, 2); ss >> g;
    ss.clear();
    ss << std::hex << hexColor.substr(5, 2); ss >> b;

    if (hexColor.length() == 9) {
        ss.clear();
        ss << std::hex << hexColor.substr(7, 2); ss >> a;
    }

    faces[faceIndex][3] = r;
    faces[faceIndex][4] = g;
    faces[faceIndex][5] = b;
    faces[faceIndex][6] = a;
}

std::string Object3d::getFaceColor(int faceIndex) const {
    if (faceIndex < 0 || faceIndex >= faces.size()) {
        throw std::out_of_range("Invalid face index");
    }

    int r = faces[faceIndex][3];
    int g = faces[faceIndex][4];
    int b = faces[faceIndex][5];
    int a = faces[faceIndex][6];

    std::stringstream ss;
    ss << "#";
    ss << std::hex << std::setw(2) << std::setfill('0') << r;
    ss << std::hex << std::setw(2) << std::setfill('0') << g;
    ss << std::hex << std::setw(2) << std::setfill('0') << b;
    ss << std::hex << std::setw(2) << std::setfill('0') << a;

    return ss.str();
}

void Object3d::setFacesColor(const std::string& hexColor) {
    if (hexColor.length() != 7 && hexColor.length() != 9 || hexColor[0] != '#') {
        return;
    }

    int r, g, b, a = 255;

    std::stringstream ss;
    ss << std::hex << hexColor.substr(1, 2); ss >> r;
    ss.clear();
    ss << std::hex << hexColor.substr(3, 2); ss >> g;
    ss.clear();
    ss << std::hex << hexColor.substr(5, 2); ss >> b;

    if (hexColor.length() == 9) {
        ss.clear();
        ss << std::hex << hexColor.substr(7, 2); ss >> a;
    }

    for (auto& face : faces) {
        face[3] = r;  // Red
        face[4] = g;  // Green
        face[5] = b;  // Blue
        face[6] = a;  // Alpha
    }
}

std::array<Vector3, 3> Object3d::getFaceVerticesForEditing(int faceIndex) const {
    if (faceIndex < 0 || faceIndex >= faces.size()) {
        throw std::out_of_range("Invalid face index");
    }

    const auto& face = faces[faceIndex];

    return {vertices[face[0]], vertices[face[1]], vertices[face[2]]};
}

void Object3d::updateFaceVertex(int faceIndex, int vertexPosition, const Vector3& newVertexPosition) {
    if (faceIndex < 0 || faceIndex >= faces.size()) {
        throw std::out_of_range("Invalid face index");
    }
    if (vertexPosition < 0 || vertexPosition > 2) {
        throw std::out_of_range("Invalid vertex position (must be 0, 1, or 2)");
    }

    const auto& face = faces[faceIndex];
    int vertexIndex = face[vertexPosition];

    // Update the vertex in the main vertices array
    vertices[vertexIndex] = newVertexPosition;
}

void Object3d::addVertex() {
    if (vertices.empty()) {
        throw std::runtime_error("Cannot calculate the center of an empty object.");
    }

    Vector3 center(0.0f, 0.0f, 0.0f);
    for (const auto& vertex : vertices) {
        center.x += vertex.x;
        center.y += vertex.y;
        center.z += vertex.z;
    }

    center.x /= vertices.size();
    center.y /= vertices.size();
    center.z /= vertices.size();

    vertices.push_back(center);
}

void Object3d::updateVertex(int vertexIndex, const Vector3& newVertexPosition) {
    if (vertexIndex < 0 || vertexIndex >= vertices.size()) {
        throw std::out_of_range("Invalid vertex index");
    }

    vertices[vertexIndex] = newVertexPosition;
}

void Object3d::deleteVertex(int vertexIndex) {
    if (vertexIndex < 0 || vertexIndex >= vertices.size()) {
        throw std::out_of_range("Invalid vertex index");
    }

    vertices.erase(vertices.begin() + vertexIndex);

    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [vertexIndex](const std::pair<int, int>& edge) {
            return edge.first == vertexIndex || edge.second == vertexIndex;
        }),
        edges.end());

    for (auto& edge : edges) {
        if (edge.first > vertexIndex) edge.first--;
        if (edge.second > vertexIndex) edge.second--;
    }

    faces.erase(std::remove_if(faces.begin(), faces.end(),
        [vertexIndex](const std::array<int, 7>& face) {
            return face[0] == vertexIndex || face[1] == vertexIndex || face[2] == vertexIndex;
        }),
        faces.end());

    for (auto& face : faces) {
        for (int i = 0; i < 3; ++i) {
            if (face[i] > vertexIndex) face[i]--;
        }
    }
}

void Object3d::setVertices(const std::vector<Vector3> &vector) {
    vertices = vector;
}

BoundingBox Object3d::getBoundingBox() const {
    if (vertices.empty()) {
        // Return an invalid bounding box if there are no vertices
        return BoundingBox{
            Vector3(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()),
            Vector3(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest())
        };
    }

    // Initialize the bounding box with the first transformed vertex
    Vector3 firstTransformedVertex = transform * vertices[0];
    BoundingBox bbox;
    bbox.min = firstTransformedVertex;
    bbox.max = firstTransformedVertex;

    // Loop through all vertices and find the min/max points
    for (const Vector3& vertex : vertices) {
        // Transform each vertex by the object's transform matrix
        Vector3 transformedVertex = transform * vertex;

        // Update the bounding box min and max coordinates
        bbox.min.x = std::min(bbox.min.x, transformedVertex.x);
        bbox.min.y = std::min(bbox.min.y, transformedVertex.y);
        bbox.min.z = std::min(bbox.min.z, transformedVertex.z);

        bbox.max.x = std::max(bbox.max.x, transformedVertex.x);
        bbox.max.y = std::max(bbox.max.y, transformedVertex.y);
        bbox.max.z = std::max(bbox.max.z, transformedVertex.z);
    }

    return bbox;
}


void Object3d::setPosition(Vector3& newPos) {
    position = newPos;
    Matrix4 rotationMatrix = Matrix4::rotationZ(rotation.z) *
                             Matrix4::rotationY(rotation.y) *
                             Matrix4::rotationX(rotation.x);
    Matrix4 scaleMatrix = Matrix4::scale(scale.x, scale.y, scale.z);
    Matrix4 translationMatrix = Matrix4::translation(position.x, position.y, position.z);
    transform = translationMatrix * rotationMatrix * scaleMatrix;
}

bool Object3d::isFaceSelected(unsigned int faceIndex) const {
    return std::find(selectedFaces.begin(), selectedFaces.end(), faceIndex) != selectedFaces.end();
}

bool Object3d::isFaceHovered(unsigned int faceIndex) const {
    return std::find(hoveredFaces.begin(), hoveredFaces.end(), faceIndex) != hoveredFaces.end();
}

bool Object3d::isVertexSelected(unsigned int vertexIndex) const {
    return std::find(selectedVertices.begin(), selectedVertices.end(), vertexIndex) != selectedVertices.end();
}

bool Object3d::isVertexHovered(unsigned int vertexIndex) const {
    return std::find(hoveredVertices.begin(), hoveredVertices.end(), vertexIndex) != hoveredVertices.end();
}

void Object3d::createFace() {
    if (selectedVertices.size() < 3) {
        return;
    }

    int v1 = selectedVertices[0];
    int v2 = selectedVertices[1];
    int v3 = selectedVertices[2];

    faces.push_back({v1, v2, v3, 255, 255, 255, 255});
}

void Object3d::createEdge() {
    if (selectedVertices.size() != 2) {
        throw std::runtime_error("Exactly 2 vertices must be selected to create an edge.");
    }

    std::pair<unsigned int, unsigned int> edge = {
        selectedVertices[0],
        selectedVertices[1]
    };

    edges.push_back(edge);
}

void Object3d::moveFaces(const Vector3& translation) {
    std::unordered_set<int> updatedVertices; // Track updated vertices by their indices

    for (unsigned int faceIndex : selectedFaces) {
        if (faceIndex >= faces.size()) {
            throw std::out_of_range("Invalid face index in selectedFaces");
        }

        const auto& face = faces[faceIndex];

        for (int i = 0; i < 3; ++i) {
            int vertexIndex = face[i];
            if (updatedVertices.find(vertexIndex) == updatedVertices.end()) {
                vertices[vertexIndex] = vertices[vertexIndex] + translation;
                updatedVertices.insert(vertexIndex); // Mark the vertex as updated
            }
        }
    }
}

void Object3d::rotateFaces(float angle, char axis) {
    if (selectedFaces.empty()) return;

    Vector3 centroid(0.0f, 0.0f, 0.0f);
    int totalVertices = 0;

    for (unsigned int faceIndex : selectedFaces) {
        if (faceIndex >= faces.size()) {
            throw std::out_of_range("Invalid face index in selectedFaces");
        }

        const auto& face = faces[faceIndex];
        for (int i = 0; i < 3; ++i) {
            centroid = centroid + vertices[face[i]];
        }
        totalVertices += 3;
    }

    centroid = centroid / static_cast<float>(totalVertices);

    Matrix4 rotationMatrix;
    switch (axis) {
        case 'x':
            rotationMatrix = Matrix4::rotationX(angle);
        break;
        case 'y':
            rotationMatrix = Matrix4::rotationY(angle);
        break;
        case 'z':
            rotationMatrix = Matrix4::rotationZ(angle);
        break;
        default:
            throw std::invalid_argument("Invalid rotation axis");
    }

    std::unordered_set<int> updatedVertices; // Track updated vertices

    for (unsigned int faceIndex : selectedFaces) {
        const auto& face = faces[faceIndex];

        for (int i = 0; i < 3; ++i) {
            int vertexIndex = face[i];

            if (updatedVertices.find(vertexIndex) == updatedVertices.end()) {
                // Translate vertex to origin relative to centroid
                Vector3 relativePosition = vertices[vertexIndex] - centroid;

                // Apply rotation
                Vector3 rotatedPosition = rotationMatrix * relativePosition;

                // Translate back to original position
                vertices[vertexIndex] = rotatedPosition + centroid;

                updatedVertices.insert(vertexIndex); // Mark as updated
            }
        }
    }
}

void Object3d::scaleFaces(float delta) {
    if (selectedFaces.empty()) return;

    if (delta < -1.0f || delta > 1.0f) {
        throw std::invalid_argument("Delta must be between -1 and 1");
    }

    Vector3 centroid(0.0f, 0.0f, 0.0f);
    int totalVertices = 0;

    for (unsigned int faceIndex : selectedFaces) {
        if (faceIndex >= faces.size()) {
            throw std::out_of_range("Invalid face index in selectedFaces");
        }

        const auto& face = faces[faceIndex];
        for (int i = 0; i < 3; ++i) {
            centroid = centroid + vertices[face[i]];
        }
        totalVertices += 3;
    }

    centroid = centroid / static_cast<float>(totalVertices);

    std::unordered_set<int> updatedVertices; // Track updated vertices

    for (unsigned int faceIndex : selectedFaces) {
        const auto& face = faces[faceIndex];

        for (int i = 0; i < 3; ++i) {
            int vertexIndex = face[i];

            if (updatedVertices.find(vertexIndex) == updatedVertices.end()) {
                Vector3 relativePosition = vertices[vertexIndex] - centroid;

                float scaleFactor = 1.0f + delta;

                Vector3 scaledPosition = relativePosition * scaleFactor;

                vertices[vertexIndex] = scaledPosition + centroid;

                updatedVertices.insert(vertexIndex);
            }
        }
    }
}

std::vector<std::array<float, 3>> Object3d::getVerticesForJson() {
    std::vector<std::array<float, 3>> arrayVertices;
    for (Vector3 vertex : vertices) {
        arrayVertices.push_back({ vertex.x, vertex.y, vertex.z });
    }
    return arrayVertices;
}

void Object3d::inverseFaceNormals() {
    for (unsigned int faceIndex : selectedFaces) {
        if (faceIndex < faces.size()) {
            std::swap(faces[faceIndex][0], faces[faceIndex][2]);
        }
    }
}

bool Object3d::hasEdgeFromSelection() const {
    for (const auto& edge : edges) {
        bool firstFound = std::find(selectedVertices.begin(), selectedVertices.end(), edge.first) != selectedVertices.end();
        bool secondFound = std::find(selectedVertices.begin(), selectedVertices.end(), edge.second) != selectedVertices.end();

        if (firstFound && secondFound) {
            return true;
        }
    }
    return false;
}

bool Object3d::hasFaceFromSelection() const {
    for (const auto& face : faces) {
        bool allVerticesFound = true;

        for (int i = 0; i < 3; ++i) {
            if (std::find(selectedVertices.begin(), selectedVertices.end(), face[i]) == selectedVertices.end()) {
                allVerticesFound = false;
                break;
            }
        }

        if (allVerticesFound) {
            return true;
        }
    }
    return false;
}

void Object3d::deleteFaceByIndex(int faceIndex) {
    if (faceIndex < 0 || faceIndex >= faces.size()) {
        throw std::out_of_range("Invalid face index");
    }

    faces.erase(faces.begin() + faceIndex);

    // Update face selections to reflect the removal
    auto updateIndex = [faceIndex](unsigned int& index) {
        if (index > faceIndex) {
            index--;
        }
    };

    selectedFaces.erase(
        std::remove_if(selectedFaces.begin(), selectedFaces.end(),
                       [faceIndex](unsigned int index) { return index == faceIndex; }),
        selectedFaces.end()
    );
    hoveredFaces.erase(
        std::remove_if(hoveredFaces.begin(), hoveredFaces.end(),
                       [faceIndex](unsigned int index) { return index == faceIndex; }),
        hoveredFaces.end()
    );

    std::for_each(selectedFaces.begin(), selectedFaces.end(), updateIndex);
    std::for_each(hoveredFaces.begin(), hoveredFaces.end(), updateIndex);
}