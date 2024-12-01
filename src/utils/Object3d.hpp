#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include "Matrix4.hpp"

class Object3d {
public:
    Matrix4 transform;

    Object3d() : transform(Matrix4::identity()) {}

    virtual void draw(sf::RenderWindow& window) = 0;

    void setTransform(const Matrix4& newTransform) {
        transform = newTransform;
    }
};

#endif