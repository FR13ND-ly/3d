#ifndef GRIDPLANE_HPP
#define GRIDPLANE_HPP

#include "Object3D.hpp"
#include "../../utils/Vector3.hpp"
#include <vector>

class GridPlane : public Object3d {
public:
    GridPlane() {
        calculateVertices();
        calculateEdges();
        calculateFaces();
    }

private:
    const float gridSize = 20.0f;  // Size of the grid (length and width)
    const float cellSize = .5f;    // Size of each square cell
    const float lineThickness = 0.02f; // Thickness of the grid lines

    void calculateVertices() {
        vertices.clear();
        float halfGrid = gridSize / 2.0f;

        for (float z = -halfGrid; z <= halfGrid; z += cellSize) {
            for (float x = -halfGrid; x <= halfGrid; x += cellSize) {
                vertices.emplace_back(x, 0.0f, z);
            }
        }
    }

    void calculateEdges() {
        edges.clear();
        // int rows = static_cast<int>(gridSize / cellSize) + 1;
        //
        // for (int z = 0; z < rows; ++z) {
        //     for (int x = 0; x < rows; ++x) {
        //         int currentIndex = z * rows + x;
        //
        //         // Add horizontal edge
        //         if (x < rows - 1) {
        //             edges.emplace_back(currentIndex, currentIndex + 1);
        //         }
        //
        //         // Add vertical edge
        //         if (z < rows - 1) {
        //             edges.emplace_back(currentIndex, currentIndex + rows);
        //         }
        //     }
        // }
    }

    void calculateFaces() {
        faces.clear();
        int rows = static_cast<int>(gridSize / cellSize) + 1;

        for (int z = 0; z < rows; ++z) {
            for (int x = 0; x < rows; ++x) {
                int currentIndex = z * rows + x;

                // Add vertical line faces (z-axis edges)
                if (z < rows - 1) {
                    int nextIndexZ = currentIndex + rows;
                    addLineFace(currentIndex, nextIndexZ, 'z');
                }

                // Add horizontal line faces (x-axis edges)
                if (x < rows - 1) {
                    int nextIndexX = currentIndex + 1;
                    addLineFace(currentIndex, nextIndexX, 'x');
                }
            }
        }
    }

    void addLineFace(int startIndex, int endIndex, char axis) {
        Vector3 start = vertices[startIndex];
        Vector3 end = vertices[endIndex];
        Vector3 offset;

        // Determine offset direction based on the axis
        if (axis == 'x') {
            offset = Vector3(0.0f, 0.0f, lineThickness / 2.0f);
        } else if (axis == 'z') {
            offset = Vector3(lineThickness / 2.0f, 0.0f, 0.0f);
        }

        // Define the four vertices for the line segment face
        int v0 = vertices.size();
        vertices.push_back(start - offset);
        vertices.push_back(start + offset);
        vertices.push_back(end - offset);
        vertices.push_back(end + offset);

        // Add the two triangles to form the quad
        faces.push_back({v0, v0 + 1, v0 + 2, 128, 128, 128});
        faces.push_back({v0 + 1, v0 + 3, v0 + 2, 128, 128, 128});
    }
};

#endif
