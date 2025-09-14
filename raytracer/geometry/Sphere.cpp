#include "Sphere.hpp"
#include "../utilities/Ray.hpp"
#include "../utilities/ShadeInfo.hpp"
#include "../utilities/BBox.hpp"
#include "../utilities/Point3D.hpp"
#include "../utilities/Vector3D.hpp"
#include <iostream>
#include <cmath> 

Sphere::Sphere(){
    //creates sphere with 0 radius at (0,0,0)
    this->c = Point3D();
    this->r = 0;
}

Sphere::Sphere(const Point3D &center, float radius){
    //creates sphere of radius at a point center
    this->c = center;
    this->r = radius;
}

Sphere::Sphere(const Sphere &object){
    //copy constructor
    this->c = object.c;
    this->r = object.r;
}

Sphere &Sphere::operator=(const Sphere &rhs){
    //assignment constructor
    this->c = rhs.c;
    this->r = rhs.r;
    return *this;
} 

std::string Sphere::to_string() const{
    // used to print out sphere info
    std::string result = "Center: " + this->c.to_string() + "\n" + "radius: " + std::to_string(this->r) + "\n";
    return result;
}

BBox Sphere::getBBox() const {
    // creates a Bounding box of length 2*radius
    // with the center of circle in middle
    return BBox(this->c - Vector3D(this->r, this->r, this->r),
        this->c + Vector3D(this->r, this->r, this->r), this);
    
}

bool Sphere::hit(const Ray &ray, float &t, ShadeInfo &s) const{
    /* motivation taken from 
    https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection */
    
    
    // initialising quadratic equation values
    constexpr double a = 1; // the dot product of two unit vectors is 1
    const double b = 2 * ray.d * Vector3D(ray.o - this->c);
    const double c = Vector3D(ray.o-this->c) * Vector3D(ray.o-this->c) - this->r* this->r;

    const double det = b * b - 4 * a * c;
    
    // case when ray doesn't intersect
    if (det < 0) {
        return false;
    }
    
    //case when ray has one intersection
    if (det == 0) {
    
        t = static_cast<float> (-b / (2*a));
    
        s.hit = true;
        s.hit_point = ray.o + t * ray.d;

        //calculating normal
        Vector3D normal = (s.hit_point - this->c);
        normal.normalize();
        s.normal = normal;

        s.ray = ray;
        s.depth += 1;
        s.t = t;
        s.material_ptr = this->material_ptr;
        return true;
    }

    //case when there are two intersection
    if (det > 0) {
        const double t1 = (-b - sqrt(det)) / (2*a);
        const double t2 = (-b + sqrt(det)) / (2*a);
        
        //we only concern ourselves with positive values of t
        //and choose the one closest i.e minimum
        if (t1 < 0 && t2 < 0) {
            return false;
        }

        if (t1 >= 0 && t2 < 0) {
            t = static_cast<float>(t1);
        }
        else if (t1 < 0 && t2 >= 0) {
            t = static_cast<float>(t2);
        }
        else {
            t = static_cast<float>(std::min(t1, t2));
        }

        s.hit = true;
        s.hit_point = ray.o + t * ray.d;

        //calculating normal
        Vector3D normal = (s.hit_point - this->c);
        normal.normalize();
        s.normal = normal;

        s.ray = ray;
        s.depth += 1;
        s.t = t;
        s.material_ptr = this->material_ptr;
        return true;
    }
    return false;
}