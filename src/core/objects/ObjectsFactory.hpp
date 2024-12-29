#ifndef OBJECTS_FACTORY_HPP
#define OBJECTS_FACTORY_HPP

#include "CustomShape.hpp"

class ObjectsFactory {
public:
    static std::shared_ptr<Object3d> combineObjects(const std::vector<std::shared_ptr<Object3d>>& objects);

    std::shared_ptr<Object3d> createObject(const std::string& objectType, float param1, int param2);

    static CustomShape createCube(float size);
    static CustomShape createSphere(float radius, int segments);
    static CustomShape createCylinder(float radius, int segments);
    static CustomShape createCone(float radius, int segments);
    static CustomShape createTriangle();
    static CustomShape createTorus();
    static CustomShape createCustomShape(const std::vector<Vector3>& vertices, const std::vector<std::array<int, 7>>& faces);

    static CustomShape import();
};

#endif