#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include "../../utils/Matrix4.hpp"
#include "../../utils/Vector3.hpp"

class Object3d {
public:
    Matrix4 transform;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    bool isSelected = false;

    Object3d()
        : transform(Matrix4::identity()), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}

    virtual ~Object3d() = default;

    virtual std::vector<std::array<int, 6>> getSortedFaces(const std::vector<Vector3>& transformedVertices) const {
        std::vector<std::tuple<float, std::array<int, 6>>> faceDepths;

        for (const auto& face : faces) {
            // Calculate the average depth of the triangle
            float zAvg = (
                transformedVertices[face[0]].z +
                transformedVertices[face[1]].z +
                transformedVertices[face[2]].z
            ) / 3.0f;

            faceDepths.emplace_back(zAvg, face);
        }

        // Sort faces by depth (farther faces first)
        std::sort(faceDepths.begin(), faceDepths.end(), [](const auto& a, const auto& b) {
            return std::get<0>(a) > std::get<0>(b);
        });

        // Extract the sorted faces
        std::vector<std::array<int, 6>> sortedFaces;
        for (const auto& [_, face] : faceDepths) {
            sortedFaces.push_back(face);
        }

        return sortedFaces;
    }

    void setTransform(const Matrix4& newTransform) {
        transform = newTransform;
    }

    void rotate(float angle, char axis) {
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
                break;
        }

        transform = transform * rotationMatrix;
    }

    // void scaleObject(float sx, float sy, float sz) {
    //     Matrix4 scaleMatrix = Matrix4::scaling(sx, sy, sz);
    //     transform = transform * scaleMatrix;
    //     scale = Vector3(sx, sy, sz);
    // }

    void translate(const Vector3& translation) {
        Matrix4 translationMatrix = Matrix4::translation(translation.x, translation.y, translation.z);
        transform = transform * translationMatrix;
        position = position + translation;
    }

    Vector3 getPosition() const {
        return position;
    }

    Vector3 getRotation() const {
        return rotation;
    }

    Vector3 getScale() const {
        return scale;
    }

    Matrix4 getTransformation() const {
        return transform;
    }

    std::vector<Vector3> getVertices() {
        return vertices;
    }

    std::vector<std::pair<int, int> > getEdges() {
        return edges;
    }

    std::vector<std::array<int, 6>> getFaces() {
        return faces;
    }
protected:
    std::vector<Vector3> vertices;
    std::vector<std::pair<int, int>> edges;
    std::vector<std::array<int, 6>> faces;
};

#endif
