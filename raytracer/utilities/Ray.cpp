#include "Ray.hpp"

Ray::Ray() {
    o.x = 0;
    o.y = 0;
    o.z = 0;

    d.x = 0;
    d.y = 0;
    d.z = 0;

    d.normalize();

    w = 1;

    color.r = 1;
    color.g = 1;
    color.b = 1;
}

Ray::Ray(const Point3D &origin, const Vector3D &dir) {
    o = origin;
    d = dir;
    d.normalize();
    w = 1;

    color.r = 1;
    color.g = 1;
    color.b = 1;
}

Ray::Ray(const Point3D &origin, const Vector3D &dir, const RGBColor &clr) {
    o = origin;
    d = dir;
    d.normalize();
    w = 1;
    
    color.r = clr.r;
    color.g = clr.g;
    color.b = clr.b;
}

std::string Ray::to_string() const {
    std::string result = "origin: " + o.to_string() + "\n" + "direction: " + d.to_string() + "\n" + "weightage: " + std::to_string(w);
    return result;
}