#pragma once

/**
   This file declares the Material class which is an abstract class for concrete
   materials to inherit from.

   Courtesy Kevin Suffern.
*/

class RGBColor;
class ShadeInfo;

class Material {
protected:
  float r_ind;  // specify significance for the color of the reflected ray
  float inc_ind;  // // specify significance for the color of the incident ray
public:
  // Constructors.
  Material() = default; // does nothing.

  // Copy constuctor and assignment operator.
  Material(const Material &other) = default;
  Material &operator=(const Material &other) = default;

  // Desctructor.
  virtual ~Material() = default;

  // Get color.
  virtual RGBColor shade(const ShadeInfo &sinfo) const = 0;
  virtual float get_r_index() const = 0;
  virtual float get_inc_index() const = 0;
};
