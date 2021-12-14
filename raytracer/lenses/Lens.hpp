#pragma once

#include "../utilities/Point3D.hpp"
#include "../utilities/Vector3D.hpp"
#include <random>

class Lens {
public:
    Point3D origin; // by default, initialized to (0, 0, 0)
    Vector3D normal; // by default, initialized along the z-axis
    float radius; // by default, initialized to 5.0
    
    // Random number generators for values of r & theta
    // where 0 < r < radius and 0 < theta < 360
    std::mt19937 radius_generator;
    std::mt19937 angle_generator;
    std::uniform_real_distribution<float> radius_distribution;
    std::uniform_int_distribution<int> angle_distribution;

    // Constructors
    Lens();
    Lens(const Point3D origin, const Vector3D normal, const float radius);

    // Destructor
    ~Lens() = default;

    // Returns a random point on the surface of the lens
    Point3D get_random_point();

    Vector3D get_direction(const Point3D &p);
};