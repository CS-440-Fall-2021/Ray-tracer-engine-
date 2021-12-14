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
    
    float _a = ray.d * ray.d;  
    float _b = 2 * ray.d * Vector3D(ray.o- this->c); 
    float _c = Vector3D(ray.o-this->c) * Vector3D(ray.o-this->c) - this->r* this->r;
    
    float det = _b * _b - 4 * _a * _c;
    
    // case when ray doesn't intersect
    if (det < 0){
        return false;
    }
    
    //case when ray has one intersection
    else if (det == 0){
    
        t = (-_b-sqrt(det))/(2*_a) ;
    
        s.hit = true;
        s.hit_point = ray.o + t * ray.d;

        //calculating normal
        Vector3D normal = (s.hit_point - this->c);
        normal.normalize();
        s.normal = normal;

        s.ray = ray;
        s.depth += 1;
        s.t = (-sqrt(det) - _b)/(2*_a);
        s.material_ptr = this->material_ptr;
        return true;
    }

    //case when there are two intersection
    else{
        float t1 = (-sqrt(det) - _b)/(2*_a);
        float t2 = (sqrt(det) - _b)/(2*_a);
        
        //we only concern ourselves with positive values of t
        //and choose the one closest i.e minimum
        if (t1 < 0 && t2 < 0){
            return false;
        }
        else {
        
            if (t1 >= 0 && t2 < 0){
                t = t1 ;
                
            }
            else if (t1 < 0 && t2 >= 0){
                t = t2 ;
                
            }
            else{
                t= std::min(t1,t2);
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
    }
    return false;
}