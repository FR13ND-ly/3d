#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include "../../utils/Matrix4.hpp"
#include "../../utils/Vector3.hpp"
#include <SFML/Graphics.hpp>

class Object3d {
public:
    Matrix4 transform;  // Transformation matrix for position, rotation, and scaling
    Vector3 position;   // Object's position (to help with transformations)
    Vector3 rotation;   // Object's rotation (in degrees for each axis)
    Vector3 scale;      // Object's scale (default: (1, 1, 1))

    Object3d()
        : transform(Matrix4::identity()), position(0.0f, 0.0f, 0.0f), rotation(0.0f, 0.0f, 0.0f), scale(1.0f, 1.0f, 1.0f) {}

    virtual ~Object3d() = default;

    // Pure virtual draw method to be overridden in derived classes
    virtual void draw(sf::RenderWindow& window, const Matrix4 &modelMatrix, const Matrix4 &viewMatrix, const Matrix4 &projectionMatrix) = 0;

    // Set a new transformation matrix directly
    void setTransform(const Matrix4& newTransform) {
        transform = newTransform;
    }

    // Apply a rotation on the object
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

    // Apply a translation to the object
    void translate(const Vector3& translation) {
        Matrix4 translationMatrix = Matrix4::translation(translation.x, translation.y, translation.z);
        transform = transform * translationMatrix;
        position = position + translation;
    }

    // Get the object's current position
    Vector3 getPosition() const {
        return position;
    }

    // Get the object's current rotation
    Vector3 getRotation() const {
        return rotation;
    }

    // Get the object's current scale
    Vector3 getScale() const {
        return scale;
    }

    // Get the current transformation matrix (combined translation, rotation, and scale)
    Matrix4 getTransformation() const {
        return transform;
    }
};

#endif
