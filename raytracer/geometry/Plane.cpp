#include "Plane.hpp"
#include "../utilities/Ray.hpp"
#include "../utilities/ShadeInfo.hpp"
#include "../utilities/BBox.hpp"
#include <iostream>

Plane::Plane(){
    //creates an xz-plane
    this->a = Point3D();
    this->n = Vector3D(0.0,1.0,0.0);
}

Plane::Plane(const Point3D &pt,const Vector3D &n){
    //creating the plane by defining a Point on the plane
    // and unit normal to that point
    this->a = pt;
    this->n = n;
    this->n.normalize();
}

Plane::Plane(const Plane &object){
    //copy constructor
    this->a = object.a;
    this->n = object.n;
    this->n.normalize();
}
Plane &Plane::operator=(const Plane &rhs){
    //assignment constructor
    this->a = rhs.a;
    this->n = rhs.n;
    this->n.normalize();
    return *this;
}
std::string Plane::to_string() const{
    // used to print out information about plane
    std::string result = "Point: " + a.to_string() + "\n" + 
                        "normal: " + n.to_string() + "\n";
    return result;
}

bool Plane::hit(const Ray &ray, float &t, ShadeInfo &s) const{
    /* Motivation taken from 
    https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection 
     */

     
    //setting intersect as 0
    float intersect = 0.0;


    if (ray.d * this->n == 0) { // when ray is perpendicular
        return false;
    }
    
    else {

        // calculating t where the ray intersect
        Vector3D numer = this->a - ray.o;    
        intersect = (numer * this->n)/(ray.d * this->n);

        // if intersection happens behind ray
        if (intersect < 0) {
            return false;
        }

        // intersection happens in front
        // return true and make changes to 
        // ShadeInfo and t accordingly
        t = intersect;
        s.hit = true;
        s.hit_point = ray.o + t * ray.d;
        s.normal = this->n;
        s.ray = ray;
        s.depth += 1;
        s.t = intersect;
        s.material_ptr = this->material_ptr;
        return true;
    }
    return false;
}

BBox Plane::getBBox() const{
    return BBox();
}