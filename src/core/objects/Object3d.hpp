#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include "../../utils/math/Matrix4.hpp"
#include "../../utils/math/Vector3.hpp"
#include <vector>
#include <array>
#include <tuple>
#include <algorithm>

class Object3d {
public:
    Matrix4 transform;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    bool isSelected = false;

    Object3d();
    virtual ~Object3d() = default;

    virtual std::vector<std::array<int, 7>> getSortedFaces(const std::vector<Vector3>& transformedVertices) const;

    void setTransform(const Matrix4& newTransform);
    void rotate(float angle, char axis);
    void translate(const Vector3& translation);

    Vector3 getPosition() const;
    Vector3 getRotation() const;
    Vector3 getScale() const;
    Matrix4 getTransformation() const;

    void setRotation(const Vector3& newRotation);
    void setScale(const Vector3& newScale);

    std::string getFaceColor(int faceIndex) const;
    void setFaceColor(int faceIndex, const std::string& hexColor);

    std::array<Vector3, 3> getFaceVerticesForEditing(int faceIndex) const;
    void updateFaceVertex(int faceIndex, int vertexPosition, const Vector3& newVertexPosition);


    std::vector<Vector3> getVertices();
    std::vector<std::pair<int, int>> getEdges();
    std::vector<std::array<int, 7>> getFaces();
protected:
    std::vector<Vector3> vertices;
    std::vector<std::pair<int, int>> edges;
    std::vector<std::array<int, 7>> faces;
};

#endif
