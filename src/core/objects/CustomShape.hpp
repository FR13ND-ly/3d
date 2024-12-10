#ifndef CUSTOMSHAPE_HPP
#define CUSTOMSHAPE_HPP

#include "Object3D.hpp"
#include "../../utils/Vector3.hpp"
#include <vector>

class CustomShape : public Object3d {
public:
    CustomShape() = default;

    CustomShape(const std::vector<Vector3>& vertices,
                const std::vector<std::pair<int, int>>& edges,
                const std::vector<std::array<int, 6>>& faces) {
          setVertices(vertices);
          setEdges(edges);
          setFaces(faces);
        }


    void setVertices(const std::vector<Vector3>& vertices) {
        this->vertices = vertices;
    }

    void setEdges(const std::vector<std::pair<int, int>>& edges) {
        this->edges = edges;
    }

    void setFaces(const std::vector<std::array<int, 6>>& faces) {
        this->faces = faces;
    }
};

#endif
