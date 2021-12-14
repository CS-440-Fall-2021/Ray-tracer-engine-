#include "Perspective.hpp"
#include "../utilities/Vector3D.hpp"
#include <iostream>

Perspective::Perspective() {
    pos.x = 0;
    pos.y = 0;
    pos.z = 0;
}

Perspective::Perspective(float c) {
    pos.x = c;
    pos.y = c;
    pos.z = c;
}

Perspective::Perspective(float x, float y, float z) {
    pos.x = x;
    pos.y = y;
    pos.z = z;
}

Perspective::Perspective(const Point3D &pt) {
    pos.x = pt.x;
    pos.y = pt.y;
    pos.z = pt.z;
}

Perspective::Perspective(const Perspective &camera) {
    pos = camera.pos;
}

Perspective &Perspective::operator=(const Perspective &other) {
    pos = other.pos;
    return *this;
}

Vector3D Perspective::get_direction(const Point3D &p) const {
    Vector3D result;
    result = p - pos;
    result.normalize();

    return result;
}