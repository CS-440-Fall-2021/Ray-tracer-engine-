#include "ViewPlane.hpp"

int ViewPlane::get_hres() const{
    return this->hres;
}
int ViewPlane::get_vres() const{
    return this->vres;
}

void ViewPlane::set_hres(int h){
    this->hres = h;
}

void ViewPlane::set_vres(int v){
    this->vres = v;
}


