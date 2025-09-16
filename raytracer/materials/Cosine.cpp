#include "Cosine.hpp"
#include "../utilities/ShadeInfo.hpp"

Cosine::Cosine() : Material(0.5) {
    color.r = 0;
    color.g = 0;
    color.b = 0;
}

Cosine::Cosine(const float c) : Material(0.5) {
    color.r = c;
    color.g = c;
    color.b = c;
}

Cosine::Cosine(const float r, const float g, const float b) : Material(0.5) {
    color.r = r;
    color.g = g;
    color.b = b;
}

Cosine::Cosine(const RGBColor &c) : Material(0.5) {
    color = c;
}

Cosine::Cosine(const Cosine &other)  : Material(other) {
    color = other.color;
}

Cosine &Cosine::operator=(const Cosine &other) {
    color = other.color;
    return *this;
}

RGBColor Cosine::shade(const ShadeInfo &sinfo) const {
    const float cos_theta = static_cast<float>(sinfo.normal * -sinfo.ray.d);
    return sinfo.ray.color * color * cos_theta;
}


float Cosine::get_r_index() const {
    return this->r_ind;
}