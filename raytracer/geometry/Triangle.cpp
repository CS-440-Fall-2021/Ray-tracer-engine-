#include "Triangle.hpp"
#include "../utilities/Ray.hpp"
#include "../utilities/ShadeInfo.hpp"
#include "../utilities/BBox.hpp"
#include "../utilities/Point3D.hpp"
#include "../utilities/Vector3D.hpp"
#include <iostream>
#include "../materials/Material.hpp"

Triangle::Triangle(){
    // initialize all points as (0,0,0)
    this->v0 = Point3D();
    this->v1 = Point3D();
    this->v2 = Point3D();
}

Triangle::Triangle(const Point3D &_a, const Point3D &_b, const Point3D &_c){
    //sets points as given
    this->v0 = _a;
    this->v1 = _b;
    this->v2 = _c;
}

Triangle::Triangle(const Triangle &object){
    //copy constructor
    this->v0 = object.v0;
    this->v1 = object.v1;
    this->v2 = object.v2;
}

Triangle &Triangle::operator=(const Triangle &rhs){
    //assignment constructor
    this->v0 = rhs.v0;
    this->v1 = rhs.v1;
    this->v2 = rhs.v2;
    return *this;
}

std::string Triangle::to_string() const{
    // used to print out triangle info
    std::string result = "Point 1: " + v0.to_string() + "\n" 
        + "Point 2: " + v1.to_string() + "\n"
        + "Point 3: " + v2.to_string() + "\n";
    return result;
}

BBox Triangle::getBBox() const {
    // gets a bounding box by calculating
    // element-wise max and min extents
    return BBox(min(min(v0,v1),v2),max(max(v0,v1),v2), this);
    
}

bool Triangle::hit(const Ray &ray, float &t, ShadeInfo &s) const{
    /* motivation taken from
    https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/ray-triangle-intersection-geometric-solution 
    */
    
    
    //initialising intersect as 0 
    float intersect = 0.0;

    //initializing edges
    Vector3D edge1 = v1 - v0;
    Vector3D edge2 = v2 - v1;
    Vector3D edge3 = v0 - v2;
    
    //calculating normal to plane of triangle
    Vector3D normal = edge1 ^ edge2;
    normal.normalize();

    //if ray and plane are parallel
    if (ray.d * normal == 0){
        return false;
    }

    //if they do intersect
    else{
        //calculating t
        intersect = -((Vector3D(ray.o) * normal)+ Vector3D(v0)*normal)/(ray.d * normal);
        
        //if intersection happened behind the ray
        if (intersect <= 0){
            return false;
        }
        
        // initialize vectors to check if intersection is
        // inside or outside triangle
        Point3D point = ray.o + intersect * ray.d;
        Vector3D point1 = point - v0;
        Vector3D point2 = point - v1;
        Vector3D point3 = point - v2;


        //if point of intersection inside triangle
        if (intersect > 0 && 
                normal * (edge1 ^ point1) > 0 &&
                normal * (edge2 ^ point2) > 0 &&
                normal * (edge3 ^ point3) > 0){
            t = intersect;
            s.hit = true;
            s.hit_point = point;
            s.normal = normal;
            s.ray = ray;
            s.depth += 1;
            s.t = intersect;
            s.material_ptr = this->material_ptr;
            return true;
        }

        //else since intersection outside triangle
        else{
            return false;
        }
    }
}