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


    Object3d()
        : transform(Matrix4::identity()), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}

    virtual ~Object3d() = default;

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
protected:
    std::vector<Vector3> vertices;
    std::vector<std::pair<int, int>> edges;
};

#endif
