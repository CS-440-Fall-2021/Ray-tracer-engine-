#include "Wall.hpp"
#include "../utilities/ShadeInfo.hpp"

Wall::Wall() : Material(0.1) {
    color.r = 0;
    color.g = 0;
    color.b = 0;
}

Wall::Wall(const float c) : Material(0.1) {
    color.r = c;
    color.g = c;
    color.b = c;
}

Wall::Wall(const float r, const float g, const float b) : Material(0.1) {
    color.r = r;
    color.g = g;
    color.b = b;
}

Wall::Wall(const RGBColor &c) : Material(0.1) {
    color = c;
}

Wall::Wall(const Wall &other) : Material(0.1) {
    color = other.color;
}

Wall &Wall::operator=(const Wall &other) {
    color = other.color;
    return *this;
}

RGBColor Wall::shade(const ShadeInfo &sinfo) const {
    // float cos_theta = sinfo.normal * -sinfo.ray.d;
    return sinfo.ray.color * color;;
}


float Wall::get_r_index() const {
    return this->r_ind;
}