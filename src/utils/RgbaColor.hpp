#pragma once
#include <string>
#include <vector>
#include <unordered_map>

struct RgbaColor {
    float r;
    float g;
    float b;
    float a;

    bool operator==(const RgbaColor& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
};

namespace std {
    template<>
    struct hash<RgbaColor> {
        size_t operator()(const RgbaColor& color) const {
            size_t h1 = std::hash<float>{}(color.r);
            size_t h2 = std::hash<float>{}(color.g);
            size_t h3 = std::hash<float>{}(color.b);
            size_t h4 = std::hash<float>{}(color.a);
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);
        }
    };
}