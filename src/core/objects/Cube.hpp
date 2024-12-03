#ifndef CUBE_HPP
#define CUBE_HPP

#include "Object3D.hpp"
#include "../../utils/Vector3.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Cube : public Object3d {
public:
    Cube(float size) : size(size) {
        calculateVertices();
        edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7},
        };
    }
    float size;
private:
    void calculateVertices() {
        float half = size / 2.0f;
        vertices = {
            {-half, -half, -half}, {half, -half, -half},
            {half, half, -half}, {-half, half, -half},
            {-half, -half, half}, {half, -half, half},
            {half, half, half}, {-half, half, half}
        };
    }
};

#endif
