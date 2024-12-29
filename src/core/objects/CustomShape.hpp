#ifndef CUSTOMSHAPE_HPP
#define CUSTOMSHAPE_HPP

#include "Object3D.hpp"
#include "../../utils/math/Vector3.hpp"
#include <vector>
#include <array>
#include <set>
#include <memory>
#include <utility>

class CustomShape : public Object3d {
public:
    CustomShape() = default;

    CustomShape(const std::vector<Vector3>& vertices,
                const std::vector<std::array<int, 7>>& faces);

    std::shared_ptr<Object3d> clone();

    void setVertices(const std::vector<Vector3>& vertices);
    void setFaces(const std::vector<std::array<int, 7>>& faces);

    const std::vector<std::pair<int, int>>& getEdges() const { return edges; }
private:
    void calculateEdges();
};

#endif