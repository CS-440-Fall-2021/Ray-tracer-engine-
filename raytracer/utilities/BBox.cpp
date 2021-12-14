#include "BBox.hpp"
#include "Ray.hpp"
#include "helpers.hpp"
#include "../geometry/Geometry.hpp"
#include <vector>
#include <algorithm>

BBox::BBox(const Point3D &min, const Point3D &max, const Geometry *g) {
    pmin = min;
    pmax = max;
    if (g!= NULL){
        geometrylist.push_back(g);
    }
}
BBox::BBox(const Point3D &min, const Point3D &max) {
    pmin = min;
    pmax = max;

}



std::string BBox::to_string() const {
    std::string result = "pmin: " + pmin.to_string() + "\n" + "pmax: " +  pmax.to_string();
    return result;
}

bool BBox::hit(const Ray &ray, float &t_enter, float &t_exit) const {
    /* 
    Motivation:
    https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
    */
    t_enter = (pmin.x - ray.o.x) / ray.d.x;
    t_exit = (pmax.x - ray.o.x) / ray.d.x;

    if (t_enter > t_exit) {
        swap(t_enter, t_exit);
    }

    float tymin = (pmin.y - ray.o.y) / ray.d.y; 
    float tymax = (pmax.y - ray.o.y) / ray.d.y; 
 
    if (tymin > tymax) {
        swap(tymin, tymax);
    } 
 
    if ((t_enter > tymax) || (tymin > t_exit)) 
        return false; 
 
    if (tymin > t_enter) 
        t_enter = tymin; 
 
    if (tymax < t_exit) 
        t_exit = tymax; 
 
    float tzmin = (pmin.z - ray.o.z) / ray.d.z; 
    float tzmax = (pmax.z - ray.o.z) / ray.d.z; 
 
    if (tzmin > tzmax) {
        swap(tzmin, tzmax);
    }
 
    if ((t_enter > tzmax) || (tzmin > t_exit)) 
        return false; 
 
    if (tzmin > t_enter) 
        t_enter = tzmin; 
 
    if (tzmax < t_exit) 
        t_exit = tzmax; 
 
    return true; 
}

void BBox::extend(Geometry *g) {
    BBox newbox = g->getBBox();
    this->pmax = max(this->pmax, newbox.pmax);
    this->pmin = min(this->pmin,newbox.pmin);
    geometrylist.push_back(g);

}

void BBox::extend(const BBox& b){
    this->pmax = max(this->pmax,b.pmax);
    this->pmin = min(this->pmin,b.pmin);
    int size_given = sizeof(b.geometrylist)/sizeof(b.geometrylist[0]);
    int size_exist = sizeof(this->geometrylist)/sizeof(this->geometrylist[0]);
    std::vector<const Geometry *> mixed(size_exist+size_given);
    std::vector<const Geometry *>::iterator it;

    it=set_union (this->geometrylist.begin(), this->geometrylist.end(), b.geometrylist.begin(), b.geometrylist.end(), mixed.begin());

    mixed.resize(it-mixed.begin()); 
    this->geometrylist = mixed;

}

bool BBox::contains(const Point3D &p){
    if (p.x>= pmin.x && p.x <= pmax.x &&
        p.y>= pmin.y && p.y <= pmax.y &&
        p.z>= pmin.z && p.z <= pmax.z ){
            return true;
        }
    else{
        return false;
    }
}

bool BBox::overlaps(const BBox& b){
    if ((this->pmax.x >= b.pmin.x && b.pmax.x >= this->pmin.x) &&
        (this->pmax.y >= b.pmin.y && b.pmax.y >= this->pmin.y) && 
        (this->pmax.z >= b.pmin.z && b.pmax.z >= this->pmin.z) ){
            return true;
        }
    else{
        return false;
    }
}   

bool BBox::overlaps(Geometry *g){
    BBox b = g->getBBox();
    if ((this->pmax.x >= b.pmin.x && b.pmax.x >= this->pmin.x) &&
        (this->pmax.y >= b.pmin.y && b.pmax.y >= this->pmin.y) && 
        (this->pmax.z >= b.pmin.z && b.pmax.z >= this->pmin.z) ){
            return true;
        }
    else{
        return false;
    }
}  

