#pragma once

#include "../utilities/Point3D.hpp"
#include "../utilities/Vector3D.hpp"

class Light
{
public:
    Point3D origin;
    Vector3D normal;
    
    /* field of light: the width of cone of light in degrees measured from the normal.
    range: [0, 90) */
    float fol;

    // Contructors
    Light(const Point3D &p, const Vector3D &v, float fol);

    // Destructor
    ~Light() = default;
};