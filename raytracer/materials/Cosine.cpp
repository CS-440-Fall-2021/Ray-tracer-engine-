#include "Cosine.hpp"
#include "../utilities/ShadeInfo.hpp"
#include <cmath>
#include <iostream>

Cosine::Cosine() {
    color.r = 0;
    color.g = 0;
    color.b = 0;
}

Cosine::Cosine(float c) {
    color.r = c;
    color.g = c;
    color.b = c;
}

Cosine::Cosine(float r, float g, float b) {
    color.r = r;
    color.g = g;
    color.b = b;
}

Cosine::Cosine(const RGBColor &c) {
    color = c;
}

Cosine::Cosine(const Cosine &other) {
    color = other.color;
}

Cosine &Cosine::operator=(const Cosine &other) {
    color = other.color;
    return *this;
}

RGBColor Cosine::shade(const ShadeInfo &sinfo) const {
    float cos_theta = sinfo.normal * -sinfo.ray.d;
    return sinfo.ray.color * color * cos_theta;  
}


float Cosine::get_inc_index() const {
    return this->inc_ind;
}

float Cosine::get_r_index() const {
    return this->r_ind;
}