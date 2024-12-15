#include "Object3d.hpp"
#include "sstream"
#include <algorithm>
#include <iomanip>

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

    // If the hex string has an alpha value (8 characters), extract it
    if (hexColor.length() == 9) {
        ss.clear();
        ss << std::hex << hexColor.substr(7, 2); ss >> a;
    }

    // Store the color and alpha values in the face array
    faces[faceIndex][3] = r;  // Red
    faces[faceIndex][4] = g;  // Green
    faces[faceIndex][5] = b;  // Blue
    faces[faceIndex][6] = a;  // Alpha
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
    // Check if there are vertices to calculate the center
    if (vertices.empty()) {
        throw std::runtime_error("Cannot calculate the center of an empty object.");
    }

    // Calculate the center of the object
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

    // Erase the vertex from the vertices array
    vertices.erase(vertices.begin() + vertexIndex);

    // Adjust edges to remove references to the deleted vertex
    edges.erase(std::remove_if(edges.begin(), edges.end(),
        [vertexIndex](const std::pair<int, int>& edge) {
            return edge.first == vertexIndex || edge.second == vertexIndex;
        }),
        edges.end());

    // Update remaining edge references
    for (auto& edge : edges) {
        if (edge.first > vertexIndex) edge.first--;
        if (edge.second > vertexIndex) edge.second--;
    }

    // Remove faces that reference the deleted vertex
    faces.erase(std::remove_if(faces.begin(), faces.end(),
        [vertexIndex](const std::array<int, 7>& face) {
            return face[0] == vertexIndex || face[1] == vertexIndex || face[2] == vertexIndex;
        }),
        faces.end());

    // Update remaining face vertex references
    for (auto& face : faces) {
        for (int i = 0; i < 3; ++i) {
            if (face[i] > vertexIndex) face[i]--;
        }
    }
}

bool Object3d::isVertexSelected(unsigned int vertexIndex) const {
    return std::find(selectedVertices.begin(), selectedVertices.end(), vertexIndex) != selectedVertices.end();
}

void Object3d::createFace() {
    if (selectedVertices.size() < 3) {
        // Not enough vertices to create a face
        return;
    }

    int v1 = selectedVertices[0];
    int v2 = selectedVertices[1];
    int v3 = selectedVertices[2];

    faces.push_back({v1, v2, v3, 255, 255, 255, 255});
}

// Create an edge from the selected vertices
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
