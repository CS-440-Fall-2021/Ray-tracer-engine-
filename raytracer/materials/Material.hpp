#pragma once
#include <stdexcept>

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
public:
  // Constructors.
  explicit Material(const float r_ind) {
      if (r_ind < 0 || r_ind > 1) {
          throw std::invalid_argument("r_ind must be in the range [0,1]");
      }
      this->r_ind = r_ind;
  }

  // Copy constructor and assignment operator.
  Material(const Material &other) = default;
  Material &operator=(const Material &other) = default;

  // Destructor.
  virtual ~Material() = default;

  // Get color.
  [[nodiscard]] virtual RGBColor shade(const ShadeInfo &sinfo) const = 0;
  [[nodiscard]] virtual float get_r_index() const = 0;
  [[nodiscard]] float get_inc_index() const {
      return 1 - r_ind;
  }
};
