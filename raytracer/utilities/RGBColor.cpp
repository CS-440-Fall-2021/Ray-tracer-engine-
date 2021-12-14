#include <cmath>
#include "RGBColor.hpp"
#include "helpers.hpp"

RGBColor::RGBColor() {
    r = 0;
    g = 0;
    b = 0;
}

RGBColor::RGBColor(float c) {
    float _c = clip(c, 0, 1);
    r = _c;
    g = _c;
    b = _c;
}

RGBColor::RGBColor(float _r, float _g, float _b) {
    r = clip(_r, 0, 1);
    g = clip(_g, 0, 1);
    b = clip(_b, 0, 1);
}

std::string RGBColor::to_string() const {
    std::string result = "(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ")";
    return result;
}

// Returns a PPM P3 format friendly string representing an RGBColor object.
std::string RGBColor::to_PPM_string() const {
    std::string result = std::to_string(static_cast<int>(r*255)) + " " + std::to_string(static_cast<int>(g*255)) + " " + std::to_string(static_cast<int>(b*255)) + " ";
    return result;
}

RGBColor RGBColor::operator+(const RGBColor &c) const {
    RGBColor result;
    result.r = clip(r + c.r, 0, 1);
    result.g = clip(g + c.g, 0, 1);
    result.b = clip(b + c.b, 0, 1);
    return result;
}

RGBColor &RGBColor::operator+=(const RGBColor &c) {
    this->r = clip(this->r + c.r, 0, 1);
    this->g = clip(this->g + c.g, 0, 1);
    this->b = clip(this->b + c.b, 0, 1);
    return *this;
}

RGBColor RGBColor::operator*(const float a) const {
    RGBColor result;
    result.r = clip(r * a, 0, 1);
    result.g = clip(g * a, 0, 1);
    result.b = clip(b * a, 0, 1);
    return result;
}

RGBColor &RGBColor::operator*=(const float a) {
    this->r = clip(this->r * a, 0, 1);
    this->g = clip(this->g * a, 0, 1);
    this->b = clip(this->b * a, 0, 1);
    return *this;
}

RGBColor RGBColor::operator/(const float a) const {
    RGBColor result;
    result.r = clip(r / a, 0, 1);
    result.g = clip(g / a, 0, 1);
    result.b = clip(b / a, 0, 1);
    return result;
}

RGBColor &RGBColor::operator/=(const float a) {
    this->r = clip(this->r / a, 0, 1);
    this->g = clip(this->g / a, 0, 1);
    this->b = clip(this->b / a, 0, 1);
    return *this;
}

RGBColor RGBColor::operator*(const RGBColor &c) const {
    RGBColor result;
    result.r = clip(r * c.r, 0, 1);
    result.g = clip(g * c.g, 0, 1);
    result.b = clip(b * c.b, 0, 1);
    return result;
}

bool RGBColor::operator==(const RGBColor &c) const {
    if (r == c.r && g == c.g && b == c.b) {
        return true;
    }
    return false;
}

RGBColor RGBColor::powc(float p) const {
    RGBColor result;
    result.r = clip(pow(r, p), 0, 1);
    result.g = clip(pow(g, p), 0, 1);
    result.b = clip(pow(b, p), 0, 1);
    return result;
}

float RGBColor::average() const {
    return (r + g + b) / 3;
}

RGBColor operator*(const float a, const RGBColor &c) {
    RGBColor result;
    result.r = clip(a * c.r, 0, 1);
    result.g = clip(a * c.g, 0, 1);
    result.b = clip(a * c.b, 0, 1);
    return result;
}