#include "Wall.hpp"
#include "../utilities/ShadeInfo.hpp"
#include <cmath>
#include <iostream>

Wall::Wall() {
    color.r = 0;
    color.g = 0;
    color.b = 0;
}

Wall::Wall(float c) {
    color.r = c;
    color.g = c;
    color.b = c;
}

Wall::Wall(float r, float g, float b) {
    color.r = r;
    color.g = g;
    color.b = b;
}

Wall::Wall(const RGBColor &c) {
    color = c;
}

Wall::Wall(const Wall &other) {
    color = other.color;
}

Wall &Wall::operator=(const Wall &other) {
    color = other.color;
    return *this;
}

RGBColor Wall::shade(const ShadeInfo &sinfo) const {
    float cos_theta = sinfo.normal * -sinfo.ray.d;
    RGBColor computed_color = sinfo.ray.color * color;
    float avg = (computed_color.r + computed_color.g + computed_color.b) / 3;

    if (avg > 0.25) {
        return computed_color * cos_theta;
    } else {
        return computed_color;
    }
}