#include "Light.hpp"

Light::Light(const Point3D &p, const Vector3D &v, float fol) {
    origin = p;
    normal = v;
    normal.normalize();
    
    this->fol = fol;
    if (this->fol < 0) {
        this->fol = this->fol * -1;
    }
    if (this->fol > 89) {
        this->fol = 89;
    }
}