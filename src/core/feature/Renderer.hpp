#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "../objects/Object3D.hpp"
#include "../../utils/math/Matrix4.hpp"
#include "../../utils/math/Vector3.hpp"
#include "../../utils/Frustum.hpp"
#include <memory>
#include <vector>

struct FaceData {
    Vector3 v1, v2, v3;
    sf::Color color;
    float depth;
    std::shared_ptr<Object3d> object;
};

class Renderer {
public:
    explicit Renderer(sf::RenderWindow& window);

    void render(const std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera);
    void renderGrid(float size, float spacing, Camera& camera);

    void setupFrustum(const Matrix4& projectionMatrix, const Matrix4& viewMatrix);

protected:
    sf::RenderWindow& window;
    Frustum frustum;

private:
    void renderFaces(const std::vector<FaceData>& facesToRender);
    void renderEdges(const std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera);

    bool isFaceCulled(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Matrix4& viewMatrix) const;
    bool isObjectInFrustum(const std::vector<Vector3>& vertices) const;
    std::vector<Vector3> transformVertices(const std::vector<Vector3>& vertices, const Matrix4& modelMatrix, const Matrix4& viewMatrix) const;
    std::vector<Vector3> projectVertices(const std::vector<Vector3>& vertices, const Matrix4& projectionMatrix) const;
    sf::Vector2f screenPosition(const Vector3& vertex) const;

    void processObject(const std::shared_ptr<Object3d>& object, Camera& camera, std::vector<FaceData>& facesToRender);
};

#endif
