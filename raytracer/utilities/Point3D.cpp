#include "Point3D.hpp"
#include "Vector3D.hpp"
#include <cmath>

Point3D::Point3D() {
    x = 0;
    y = 0;
    z = 0;
}

Point3D::Point3D(float c) {
    x = c;
    y = c;
    z = c;
}

Point3D::Point3D(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

std::string Point3D::to_string() const {
    std::string result = "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    return result;
}

Point3D Point3D::operator-() const {
    return Point3D(-x, -y, -z);
}

Vector3D Point3D::operator-(const Point3D &p) const {
    return Vector3D(x - p.x, y - p.y, z - p.z);
}

Point3D Point3D::operator+(const Vector3D &v) const {
    return Point3D(v.x + x, v.y + y, v.z + z);
}

Point3D Point3D::operator-(const Vector3D &v) const {
    return Point3D(x - v.x, y - v.y, z - v.z);
}

Point3D Point3D::operator*(const float s) const {
    return Point3D(x*s, y*s, z*s);    
}

float Point3D::distance(const Point3D &p) const {
    return pow(pow(p.x - x, 2) + pow(p.y - y, 2) + pow(p.z - z, 2), 0.5);
}

float Point3D::d_squared(const Point3D &p) const {
    return pow(Point3D::distance(p), 2);
}

Point3D operator*(const float a, const Point3D &pt) {
    return Point3D(pt.x * a, pt.y * a, pt.z * a);
}

Point3D min(const Point3D &a, const Point3D &b) {
    return Point3D(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Point3D max(const Point3D &a, const Point3D &b) {
    return Point3D(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}