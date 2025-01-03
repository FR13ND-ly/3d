#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include "../../utils/math/Matrix4.hpp"
#include "../../utils/math/Vector3.hpp"
#include <vector>
#include <array>
#include <tuple>
#include <algorithm>
#include <memory>

struct BoundingBox {
    Vector3 min;
    Vector3 max;
};

class Object3d {
public:
    Matrix4 transform;
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    bool isSelected = false;
    bool isHovered = false;

    Object3d();
    virtual ~Object3d() = default;
    virtual std::shared_ptr<Object3d> clone() const {
        return std::make_shared<Object3d>(*this);
    }

    void setVertices(const std::vector<Vector3> & vector);;

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
    void setFacesColor(const std::string& hexColor);

    std::array<Vector3, 3> getFaceVerticesForEditing(int faceIndex) const;
    void updateFaceVertex(int faceIndex, int vertexPosition, const Vector3& newVertexPosition);

    BoundingBox getBoundingBox() const;
    void addVertex();
    void updateVertex(int vertexIndex, const Vector3& newVertexPosition);
    void deleteVertex(int vertexIndex);

    void deleteFaceByIndex(int faceIndex);
    bool isFaceSelected(unsigned int i) const;
    bool isFaceHovered(unsigned int i) const;

    bool isVertexSelected(unsigned int i) const;
    bool isVertexHovered(unsigned int i) const;

    void createFace();
    void createEdge();

    void scaleFaces(float scaleFactor);
    void moveFaces(const Vector3& translation);
    void rotateFaces(float angle, char axis);

    std::vector<Vector3> getVertices();
    std::vector<std::array<float, 3>> getVerticesForJson();
    std::vector<std::pair<int, int>> getEdges();
    std::vector<std::array<int, 7>> getFaces();

    std::vector<unsigned int> selectedFaces;
    std::vector<unsigned int> hoveredFaces;

    std::vector<unsigned int> selectedVertices;
    std::vector<unsigned int> hoveredVertices;

    void inverseFaceNormals();
    bool hasEdgeFromSelection() const;
    bool hasFaceFromSelection() const;
    std::vector<Vector3> vertices;
    std::vector<std::array<int, 7>> faces;
    void setPosition(Vector3& newPos);
protected:
    std::vector<std::pair<int, int>> edges;
};

#endif
