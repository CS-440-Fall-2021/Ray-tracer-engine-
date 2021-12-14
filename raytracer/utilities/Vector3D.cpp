#include "Vector3D.hpp"
#include "Point3D.hpp"
#include <cmath>

Vector3D::Vector3D() {
    x = 0;
    y = 0;
    z = 0;
}

Vector3D::Vector3D(double c) {
    x = c;
    y = c;
    z = c;
}

Vector3D::Vector3D(double _x, double _y, double _z) {
    x = _x;
    y = _y;
    z = _z;
}

Vector3D::Vector3D(const Point3D &p) {
    x = p.x;
    y = p.y;
    z = p.z;
}

Vector3D &Vector3D::operator=(const Point3D &rhs) {
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    return *this;
}

std::string Vector3D::to_string() const {
    std::string result = "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    return result;
}

Vector3D Vector3D::operator-() const {
    return Vector3D(-x, -y, -z);
}

Vector3D Vector3D::operator+(const Vector3D &v) const {
    return Vector3D(x + v.x, y + v.y, z + v.z);
}

Vector3D &Vector3D::operator+=(const Vector3D &v) {
    this->x = this->x + v.x;
    this->y = this->y + v.y;
    this->z = this->z + v.z;
    return *this;
}

Vector3D Vector3D::operator-(const Vector3D &v) const {
    return Vector3D(v.x - x, v.y - y, v.z - z);
}

Vector3D &Vector3D::operator-=(const Vector3D &v) {
    this->x -= v.x;
    this->y -= v.y;
    this->z -= v.z;
    return *this;
}

Vector3D Vector3D::operator*(const double a) const {
    return Vector3D(x*a, y*a, z*a);
}

Vector3D Vector3D::operator/(const double a) const {
    return Vector3D(x/a, y/a, z/a);
}

double Vector3D::length() const {
    return pow((pow(x, 2) + pow(y, 2) + pow(z, 2)), 0.5);
}

double Vector3D::len_squared() const {
    return pow(Vector3D::length(), 2);
}

void Vector3D::normalize() {
    double magnitude = Vector3D::length();

    if (magnitude == 0) {
        return;
    } else {
        x = x / magnitude;
        y = y / magnitude;
        z = z / magnitude;
    }
}

double Vector3D::operator*(const Vector3D &b) const {
    return x*b.x + y*b.y + z*b.z;
}

Vector3D Vector3D::operator^(const Vector3D &v) const {
    return Vector3D((y*v.z) - (z*v.y), -((x*v.z) - (z*v.x)), (x*v.y) - (y*v.x));
}

Vector3D operator*(const double a, const Vector3D &v) {
    return Vector3D(v.x*a, v.y*a, v.z*a);
}