#include "Lens.hpp"
#include <cmath>
#include <iostream>

Lens::Lens() {
    origin = Point3D(0, 0, 0);
    normal = Vector3D(0, 0, 1);
    radius = 5;
    focal_plane = Plane(Point3D(0, 0, -25), Vector3D(0, 0, 1));

    radius_distribution = std::uniform_real_distribution<float>(0, 99);
    angle_distribution = std::uniform_int_distribution<int>(0, 359);
}

Lens::Lens(const Point3D &origin, const Vector3D &normal, const float radius, const Plane &fp) {
    this->origin = origin;
    this->normal = normal;
    this->radius = radius;
    this->focal_plane = fp;

    radius_distribution = std::uniform_real_distribution<float>(0, radius + 1);
    angle_distribution = std::uniform_int_distribution<int>(0, 360);
}

Point3D Lens::get_random_point() {
    /* Potential Bug:
       Assumes the lens is at (0, 0, 0). For other values, this will break.
    */

    float r = radius_distribution(radius_generator);
    int theta = angle_distribution(angle_generator);

    return Point3D(origin.x + (r * cos(theta)), origin.y + (r * sin(theta)), origin.z);
}

Vector3D Lens::get_direction(const Point3D &p) {
    Vector3D result;
    result = p - origin;
    result.normalize();

    return result;
}