#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <cmath>
#include <iostream> // Optional, for debugging

class Vector3 {
public:
    float x, y, z;

    Vector3(float x = 0, float y = 0, float z = 0);

    // Vector operations
    bool operator<(const Vector3& other) const;
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;
    float lengthSquared() const;

    // Vector utility functions
    Vector3 normalized() const;
    Vector3 toEulerAngles() const;
    Vector3 cross(const Vector3& other) const;
    float dot(const Vector3& other) const;
    float magnitude() const;
    float length() const;

    // Debugging function
    void print() const;

    // Comparison operators
    bool operator==(const Vector3& other) const;
    bool operator!=(const Vector3& other) const;
};

#endif // VECTOR3_HPP
