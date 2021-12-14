#include "Geometry.hpp"
#include <iostream>
#include "../materials/Material.hpp"

Geometry::Geometry(){
    //sets the material_ptr to NULL
    this->material_ptr = NULL;
}

Material* Geometry::get_material() const{
    //gets the material_ptr
    return this->material_ptr;
}

void Geometry::set_material(Material *mptr){
    //sets the material_ptr
    this->material_ptr = mptr;
}