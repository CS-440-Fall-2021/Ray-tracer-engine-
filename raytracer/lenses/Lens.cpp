#include "Lens.hpp"
#include <cmath>
#include <iostream>

Lens::Lens() {
    origin = Point3D(0, 0, 0);
    normal = Vector3D(0, 0, 1);
    radius = 5;

    radius_distribution = std::uniform_real_distribution<float>(0, 99);
    angle_distribution = std::uniform_int_distribution<int>(0, 359);
}

Lens::Lens(const Point3D origin, const Vector3D normal, const float radius) {
    this->origin = origin;
    this->normal = normal;
    this->radius = radius;

    radius_distribution = std::uniform_real_distribution<float>(0, radius);
    angle_distribution = std::uniform_int_distribution<int>(0, 359);
}

Point3D Lens::get_random_point() {
    /* Potential Bug:
       Assumes the lens is at (0, 0, 0). For other values, this will break.
    */

    float r = radius_distribution(radius_generator);
    int theta = angle_distribution(angle_generator);

    return Point3D(radius * cos(theta), radius * sin(theta), origin.z);
}