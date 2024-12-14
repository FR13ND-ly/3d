#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Camera.hpp"
#include "../objects/Object3D.hpp"
#include "../objects/Cube.hpp"
#include "../../utils/math/Matrix4.hpp"
#include "../../utils/math/Vector3.hpp"
#include "../../utils/math/Vector4.hpp"
#include <memory>

struct FaceData {
    Vector3 v1, v2, v3;
    sf::Color color;
    float depth;
    std::shared_ptr<Object3d> object;
};

class Renderer {
public:
    Renderer(sf::RenderWindow& window);

    virtual void render(std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera);
    void renderFaces(const std::vector<FaceData>& facesToRender);
    void renderEdges(std::vector<std::shared_ptr<Object3d>>& objects, Camera& camera);
    void renderGrid(float size, float spacing, Camera& camera);
protected:
    sf::RenderWindow& window;
    bool isFaceCulled(const Vector3& v1, const Vector3& v2, const Vector3& v3);
    bool isObjectInFrustum(const std::vector<Vector3>& vertices);
};

#endif
