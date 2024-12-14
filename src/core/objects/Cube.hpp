#ifndef CUBE_HPP
#define CUBE_HPP

#include "Object3D.hpp"

class Cube : public Object3d {
public:
    Cube(float sideLength);

private:
    float sideLength;

    void calculateVertices();
    void calculateFaces();
    void calculateEdges();
};

#endif