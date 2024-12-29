#ifndef ZBUFFER_HPP
#define ZBUFFER_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "../core/objects/Object3d.hpp"
#include "../core/feature/Camera.hpp"

class Scene;

class ZBuffer {
public:
    explicit ZBuffer(sf::RenderWindow& window);

    void handleClick(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera);
    bool isPointInTriangle(const sf::Vector2f& p, const sf::Vector2f& a,
                          const sf::Vector2f& b, const sf::Vector2f& c) const;
    float getDistance(const sf::Vector2f& p1, const sf::Vector2f& p2) const;

    void handleHover(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera);

private:
    sf::RenderWindow& window;
    static constexpr float VERTEX_SELECTION_RADIUS = 10.0f;
    static constexpr float FACE_SELECTION_THRESHOLD = 0.01f;

    std::vector<Vector3> transformVertices(const std::vector<Vector3>& vertices,
                                         const Matrix4& modelMatrix,
                                         const Matrix4& viewMatrix) const;
    std::vector<Vector3> projectVertices(const std::vector<Vector3>& viewSpaceVertices,
                                       const Matrix4& projectionMatrix) const;

    void handleObjectSelection(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera);
    void handleFaceSelection(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera);
    void handleVertexSelection(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera);

    void handleObjectHover(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera);
    void handleFaceHover(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera);
    void handleVertexHover(const sf::Vector2i& mousePos, Scene& scene, const Camera& camera);

    std::vector<Vector3> transformAndProjectVertices(const std::vector<Vector3>& vertices,
                                                   const Matrix4& modelMatrix,
                                                   const Matrix4& viewMatrix,
                                                   const Matrix4& projectionMatrix) const;
    sf::Vector2f screenPosition(const Vector3& vertex) const;
    float calculateBarycentric(const sf::Vector2f& p, const sf::Vector2f& a,
                             const sf::Vector2f& b, const sf::Vector2f& c) const;
};

#endif