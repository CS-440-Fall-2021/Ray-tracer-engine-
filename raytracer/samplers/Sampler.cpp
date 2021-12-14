#include "Sampler.hpp"
#include "../cameras/Camera.hpp"
#include "../utilities/Ray.hpp"
#include "../world/ViewPlane.hpp"

Sampler::Sampler() {
    this->camera_ptr = NULL;
    this->viewplane_ptr = NULL;
}

Sampler::Sampler(Camera *c_ptr, ViewPlane *v_ptr) {
    this->camera_ptr = c_ptr;
    this->viewplane_ptr = v_ptr;
}