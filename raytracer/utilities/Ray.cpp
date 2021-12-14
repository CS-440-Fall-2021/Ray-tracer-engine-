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
}

Ray::Ray(const Point3D &origin, const Vector3D &dir) {
    o = origin;
    d = dir;
    d.normalize();
    w = 1;
}

std::string Ray::to_string() const {
    std::string result = "origin: " + o.to_string() + "\n" + "direction: " + d.to_string() + "\n" + "weightage: " + std::to_string(w);
    return result;
}