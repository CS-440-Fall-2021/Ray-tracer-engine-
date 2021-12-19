#include "Matte.hpp"
#include "../utilities/ShadeInfo.hpp"
#include <cmath>
#include <iostream>

Matte::Matte() {
    color.r = 0;
    color.g = 0;
    color.b = 0;
    r_ind = 0.25;
    inc_ind = 0.75;

}

Matte::Matte(float c) {
    color.r = c;
    color.g = c;
    color.b = c;
    r_ind = 0.25;
    inc_ind = 0.75;

}

Matte::Matte(float r, float g, float b) {
    color.r = r;
    color.g = g;
    color.b = b;
    r_ind = 0.25;
    inc_ind = 0.75;

}

Matte::Matte(const RGBColor &c) {
    color = c;
    r_ind = 0.25;
    inc_ind = 0.75;
}

Matte::Matte(const Matte &other) {
    color = other.color;
    r_ind = other.r_ind;
    inc_ind = other.inc_ind;
}

Matte &Matte::operator=(const Matte &other) {
    color = other.color;
    r_ind = other.r_ind;
    inc_ind = other.inc_ind;
    return *this;
}

RGBColor Matte::shade(const ShadeInfo &sinfo) const {
    float cos_theta = sinfo.normal * -sinfo.ray.d;
    return sinfo.ray.color * color * cos_theta;  
}

float Matte::get_inc_index() const {
    return this->inc_ind;
}

float Matte::get_r_index() const {
    return this->r_ind;
}