#include "Lens.hpp"
#include <cmath>

Lens::Lens() {
    origin = Point3D(0, 0, 0);
    normal = Vector3D(0, 0, 1);
    radius = 5;

    radius_distribution = std::uniform_int_distribution(0, 99);
    angle_distribution = std::uniform_int_distribution(0, 359);
}

Lens::Lens(const Point3D origin, const Vector3D normal, const float radius) {
    this->origin = origin;
    this->normal = normal;
    this->radius = radius;
}

Point3D Lens::get_random_point() {
    float r = radius * radius_distribution(radius_generator);
    int theta = angle_distribution(angle_generator);

    return Point3D(radius * cos(theta), radius * sin(theta), origin.z);
}