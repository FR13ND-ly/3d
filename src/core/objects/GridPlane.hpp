#ifndef GRIDPLANE_HPP
#define GRIDPLANE_HPP

#include "Object3D.hpp"
#include "../../utils/math/Vector3.hpp"
#include <vector>

class GridPlane : public Object3d {
public:
    GridPlane();
private:
    const float gridSize = 20.0f;
    const float cellSize = .5f;
    const float lineThickness = 0.02f;

    void calculateVertices();
    void calculateEdges();
    void calculateFaces();
    void addLineFace(int startIndex, int endIndex, char axis);
};

#endif
