#include "Sphere.hpp"

Sphere::Sphere(float radius, int latSegments, int lonSegments)
    : radius(radius), latSegments(latSegments), lonSegments(lonSegments) {
    calculateVertices();
    calculateEdges();
    calculateFaces();
}

void Sphere::calculateVertices() {
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

void Sphere::calculateFaces() {
    faces.clear();

    for (unsigned int lat = 0; lat < latSegments; ++lat) {
        for (unsigned int lon = 0; lon < lonSegments; ++lon) {
            int first = lat * (lonSegments + 1) + lon;
            int second = first + lonSegments + 1;

            faces.push_back({first, second, first + 1, abs(static_cast<int>(255 - lon * lon)), abs(static_cast<int>(lon)), abs(static_cast<int>(lat)), 255});
            faces.push_back({second, second + 1, first + 1, abs(static_cast<int>(255 - lat * lat)), abs(static_cast<int>(lat % 255)), abs(static_cast<int>(lon % 255)), 255});
        }
    }
}

void Sphere::calculateEdges() {
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
