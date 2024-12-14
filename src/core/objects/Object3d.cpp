#include "Object3d.hpp"

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
    // Update the internal rotation vector
    rotation = newRotation;

    // Construct the new rotation matrices for each axis
    Matrix4 rotationX = Matrix4::rotationX(newRotation.x);
    Matrix4 rotationY = Matrix4::rotationY(newRotation.y);
    Matrix4 rotationZ = Matrix4::rotationZ(newRotation.z);

    // Combine the rotation matrices (order matters, e.g., ZYX or XYZ)
    Matrix4 rotationMatrix = rotationZ * rotationY * rotationX; // Assuming ZYX rotation order

    // Preserve the current position and scale
    Matrix4 translationMatrix = Matrix4::translation(position.x, position.y, position.z);
    Matrix4 scaleMatrix = Matrix4::scale(scale.x, scale.y, scale.z);
    // Rebuild the transform matrix with the new rotation
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
