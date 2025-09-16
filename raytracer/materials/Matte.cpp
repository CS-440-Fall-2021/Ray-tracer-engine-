#include "Matte.hpp"
#include "../utilities/ShadeInfo.hpp"

Matte::Matte() : Material(0.1) {
    color.r = 0;
    color.g = 0;
    color.b = 0;
    r_ind = 0.1;
}

Matte::Matte(const float c) : Material(0.1) {
    color.r = c;
    color.g = c;
    color.b = c;
}

Matte::Matte(const float r, const float g, const float b) : Material(0.1) {
    color.r = r;
    color.g = g;
    color.b = b;
}

Matte::Matte(const RGBColor &c) : Material(0.1) {
    color = c;
}

Matte::Matte(const Matte &other)  : Material(other) {
    color = other.color;
    r_ind = other.r_ind;
}

Matte &Matte::operator=(const Matte &other) {
    color = other.color;
    r_ind = other.r_ind;
    return *this;
}

RGBColor Matte::shade(const ShadeInfo &sinfo) const {
    const float cos_theta = static_cast<float>(sinfo.normal * -sinfo.ray.d);
    return sinfo.ray.color * color * cos_theta;
}

float Matte::get_r_index() const {
    return this->r_ind;
}