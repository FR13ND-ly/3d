#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include "../../utils/Matrix4.hpp"
#include "../../utils/Vector3.hpp"
#include <SFML/Graphics.hpp>

class Object3d {
public:
    Matrix4 transform;

    Object3d() : transform(Matrix4::identity()) {}

    virtual void draw(sf::RenderWindow& window, const Matrix4& mvp) = 0;

    void setTransform(const Matrix4& newTransform) {
        transform = newTransform;
    }

    void rotate(float angle, char axis) {
        Matrix4 rotationMatrix;

        switch(axis) {
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
};

#endif