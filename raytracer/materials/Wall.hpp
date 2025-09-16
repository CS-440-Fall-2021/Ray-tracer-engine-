#pragma once

#include "Material.hpp"

#include "../utilities/RGBColor.hpp"

class Wall final : public Material {
protected:
  RGBColor color; // the color of the material.

public:
  // Constructors.
  Wall();                          // set color to (0, 0, 0).
  explicit Wall(float c);                   // set color to (c, c, c).
  Wall(float r, float g, float b); // set color to (r, g, b).
  explicit Wall(const RGBColor &c);         // set color to c.

  // Copy constructor and assignment operator.
  Wall(const Wall &other);
  Wall &operator=(const Wall &other);

  // Destructor.
  ~Wall() override = default;

  /* Returned shade is: color * cos \theta.
     \theta is the angle between the normal at the hit pont and the ray.
     Assuming unit vectors, cos \theta = dot product of normal and -ray.dir.
  */
  [[nodiscard]] RGBColor shade(const ShadeInfo &sinfo) const override;

  [[nodiscard]] float get_r_index() const override;
};
