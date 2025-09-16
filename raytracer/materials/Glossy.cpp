#include "Glossy.hpp"
#include "../utilities/ShadeInfo.hpp"

Glossy::Glossy() : Material(0.75) {
    color.r = 0;
    color.g = 0;
    color.b = 0;
}

Glossy::Glossy(const float c) : Material(0.75) {
    color.r = c;
    color.g = c;
    color.b = c;
}

Glossy::Glossy(const float r, const float g, const float b) : Material(0.75) {
    color.r = r;
    color.g = g;
    color.b = b;
}

Glossy::Glossy(const RGBColor &c) : Material(0.75) {
    color = c;
}

Glossy::Glossy(const Glossy &other)  : Material(other) {
    color = other.color;
}

Glossy &Glossy::operator=(const Glossy &other) {
    color = other.color;
    r_ind = other.r_ind;
    return *this;
}

RGBColor Glossy::shade(const ShadeInfo &sinfo) const {
    const float cos_theta = static_cast<float>(sinfo.normal * -sinfo.ray.d);
    return sinfo.ray.color * color * cos_theta;  
}

float Glossy::get_r_index() const {
    return this->r_ind;
}