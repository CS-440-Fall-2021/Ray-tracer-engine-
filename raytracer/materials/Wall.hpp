#pragma once

#include "Material.hpp"

#include "../utilities/RGBColor.hpp"

class Wall : public Material {
protected:
  RGBColor color; // the color of the material.

public:
  // Constructors.
  Wall();                          // set color to (0, 0, 0).
  Wall(float c);                   // set color to (c, c, c).
  Wall(float r, float g, float b); // set color to (r, g, b).
  Wall(const RGBColor &c);         // set color to c.

  // Copy constuctor and assignment operator.
  Wall(const Wall &other);
  Wall &operator=(const Wall &other);

  // Desctructor.
  virtual ~Wall() = default;

  /* Returned shade is: color * cos \theta.
     \theta is the angle between the normal at the hit pont and the ray.
     Assuming unit vectors, cos \theta = dot product of normal and -ray.dir.
  */
  virtual RGBColor shade(const ShadeInfo &sinfo) const override;
};
