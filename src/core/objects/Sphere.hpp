#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Object3D.hpp"
#include "../../utils/Vector3.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

class Sphere : public Object3d {
public:
    Sphere(float radius, int latSegments, int lonSegments)
        : radius(radius), latSegments(latSegments), lonSegments(lonSegments) {
        calculateVerticesAndEdges();
    }

private:
    float radius;
    int latSegments;
    int lonSegments;

    void calculateVerticesAndEdges() {
        vertices.clear();
        edges.clear();

        for (int lat = 0; lat <= latSegments; ++lat) {
            float theta = M_PI * lat / latSegments;
            float sinTheta = sin(theta);
            float cosTheta = cos(theta);

            for (int lon = 0; lon <= lonSegments; ++lon) {
                float phi = 2 * M_PI * lon / lonSegments;
                float sinPhi = sin(phi);
                float cosPhi = cos(phi);

                float x = radius * sinTheta * cosPhi;
                float y = radius * sinTheta * sinPhi;
                float z = radius * cosTheta;

                vertices.push_back({x, y, z});
            }
        }

        for (int lat = 0; lat < latSegments; ++lat) {
            for (int lon = 0; lon < lonSegments; ++lon) {
                int current = lat * (lonSegments + 1) + lon;
                int next = current + 1;
                int nextLat = current + (lonSegments + 1);

                edges.emplace_back(current, next);

                if (lat < latSegments - 1) {
                    edges.emplace_back(current, nextLat);
                }
            }
        }
    }
};

#endif
