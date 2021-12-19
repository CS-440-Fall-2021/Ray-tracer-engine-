#include "Glossy.hpp"
#include "../utilities/ShadeInfo.hpp"
#include <cmath>
#include <iostream>

Glossy::Glossy() {
    color.r = 0;
    color.g = 0;
    color.b = 0;
    r_ind = 0.75;
    inc_ind = 0.25;
}

Glossy::Glossy(float c) {
    color.r = c;
    color.g = c;
    color.b = c;
    r_ind = 0.75;
    inc_ind = 0.25;
}

Glossy::Glossy(float r, float g, float b) {
    color.r = r;
    color.g = g;
    color.b = b;
    r_ind = 0.75;
    inc_ind = 0.25;
}

Glossy::Glossy(const RGBColor &c) {
    color = c;
    r_ind = 0.75;
    inc_ind = 0.25;
}

Glossy::Glossy(const Glossy &other) {
    color = other.color;
    r_ind = other.r_ind;
    inc_ind = other.inc_ind;
}

Glossy &Glossy::operator=(const Glossy &other) {
    color = other.color;
    r_ind = other.r_ind;
    inc_ind = other.inc_ind;
    return *this;
}

RGBColor Glossy::shade(const ShadeInfo &sinfo) const {
    float cos_theta = sinfo.normal * -sinfo.ray.d;
    return sinfo.ray.color * color * cos_theta;  
}

float Glossy::get_inc_index() const {
    return this->inc_ind;
}

float Glossy::get_r_index() const {
    return this->r_ind;
}