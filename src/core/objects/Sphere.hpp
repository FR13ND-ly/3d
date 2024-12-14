#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Object3D.hpp"

class Sphere : public Object3d {
public:
    Sphere(float radius, int latSegments, int lonSegments);

private:
    float radius;
    int latSegments;
    int lonSegments;

    void calculateVertices();
    void calculateFaces();
    void calculateEdges();
};

#endif
