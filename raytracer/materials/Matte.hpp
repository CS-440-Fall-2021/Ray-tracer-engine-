#pragma once

/**
   This file declares the Matte class which represents a simple Matte
   material.

   It assigns a shade according to the cosinr of the angle between the incoming
   ray direction and the normal at the hit point.

   Courtesy Kevin Suffern.
*/

#include "Material.hpp"

#include "../utilities/RGBColor.hpp"

class Matte : public Material {
protected:
  float r_ind ;    // specify significance for the color of the reflected ray
  float inc_ind ;  // // specify significance for the color of the incident ray
  RGBColor color; // the color of the material.

public:
  // Constructors.
  Matte();                          // set color to (0, 0, 0).
  Matte(float c);                   // set color to (c, c, c).
  Matte(float r, float g, float b); // set color to (r, g, b).
  Matte(const RGBColor &c);         // set color to c.

  // Copy constuctor and assignment operator.
  Matte(const Matte &other);
  Matte &operator=(const Matte &other);

  // Desctructor.
  virtual ~Matte() = default;

  /* Returned shade is: color * cos \theta.
     \theta is the angle between the normal at the hit pont and the ray.
     Assuming unit vectors, cos \theta = dot product of normal and -ray.dir.
  */
  virtual RGBColor shade(const ShadeInfo &sinfo) const override;
  
  virtual float get_r_index() const override;
  virtual float get_inc_index() const override;
};
