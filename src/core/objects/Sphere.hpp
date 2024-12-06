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
        calculateVertices();
        calculateEdges();
        calculateFaces();
    }

private:
    float radius;
    int latSegments;
    int lonSegments;

    void calculateVertices() {
        vertices.clear();

        for (unsigned int lat = 0; lat <= latSegments; ++lat) {
            float theta = lat * M_PI / latSegments;
            for (unsigned int lon = 0; lon <= lonSegments; ++lon) {
                float phi = lon * 2.0f * M_PI / lonSegments;

                float x = radius * sin(theta) * cos(phi);
                float y = radius * sin(theta) * sin(phi);
                float z = radius * cos(theta);

                vertices.push_back(Vector3(x, y, z));
            }
        }
    }

    void calculateFaces() {
        faces.clear();

        for (unsigned int lat = 0; lat < latSegments; ++lat) {
            for (unsigned int lon = 0; lon < lonSegments; ++lon) {
                int first = lat * (lonSegments + 1) + lon;
                int second = first + lonSegments + 1;

                faces.push_back({first, second, first + 1, abs(255 - lon * lon), abs(lon), abs(lat)});
                faces.push_back({second, second + 1, first + 1, abs(255 - lat * lat), abs(lat % 255), abs(lon % 255)});
            }
        }
    }

    void calculateEdges() {
        edges.clear();

        for (unsigned int lat = 0; lat < latSegments; ++lat) {
            for (unsigned int lon = 0; lon < lonSegments; ++lon) {
                int first = lat * (lonSegments + 1) + lon;
                int second = first + lonSegments + 1;
                edges.push_back({first, first + 1});
                edges.push_back({first, second});
                edges.push_back({second, second + 1});
            }
        }
    }
};

#endif
