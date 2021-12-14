#include "Simple.hpp"
#include "../utilities/Point3D.hpp"
#include "../utilities/Vector3D.hpp"
#include "../utilities/Ray.hpp"
#include "../cameras/Camera.hpp"
#include "../world/ViewPlane.hpp"
#include <iostream>

Simple::Simple(Camera *c_ptr, ViewPlane *v_ptr) {
    this->camera_ptr = c_ptr;
    this->viewplane_ptr = v_ptr;
}

Simple::Simple(const Simple &camera) {
    this->camera_ptr = camera.camera_ptr;
    this->viewplane_ptr = camera.viewplane_ptr;
}

Simple &Simple::operator=(const Simple &other) {
    this->camera_ptr = other.camera_ptr;
    this->viewplane_ptr = other.viewplane_ptr;
    return *this;
}

std::vector<Ray> Simple::get_rays(int px, int py) const {
    
    // finding middle of a pixel
    float midpx = px + 0.5;
    float midpy = py + 0.5;

    // calculating viewplane dimensions in world coordinates
    float viewplane_width = (viewplane_ptr->bottom_right.x - viewplane_ptr->top_left.x);
    float viewplane_height = (viewplane_ptr->bottom_right.y - viewplane_ptr->top_left.y);

    // calculating the dimensions of each viewplane pixel in pixel coordinates
    float pix_width = viewplane_width / viewplane_ptr->vres;
    float pix_height= viewplane_height / viewplane_ptr->hres; 

    Point3D origin;
    
    // mapping the physical pixel to the viewplane pixel
    origin.x = midpx * pix_width + viewplane_ptr->top_left.x;
    origin.y = midpy * pix_height + viewplane_ptr->top_left.y;
    origin.z = viewplane_ptr->top_left.z;
    
    Vector3D direction;
    direction = camera_ptr->get_direction(origin);

    Ray r(origin, direction);

    std::vector<Ray> result;
    result.push_back(r);

    return result;
}