#pragma once

/**
   This file declares the World class which contains all the information about
   the scene - geometry and materials, lights, viewplane, camera, samplers, and
   acceleration structures.

   It also traces rays through the scene.

   Courtesy Kevin Suffern.
*/

#include <vector>

#include "../utilities/RGBColor.hpp"
#include "../lights/Light.hpp"
#include "../utilities/BBox.hpp"

#include "ViewPlane.hpp"

class Camera;
class Geometry;
class Ray;
class Sampler;
class ShadeInfo;
class Lens;
class BBox;

class World {
public:
  ViewPlane vplane;
  RGBColor bg_color;
  std::vector<Geometry *> geometry;
  std::vector<Geometry *> walls;
  Camera *camera_ptr;
  Sampler *sampler_ptr;
  Lens *lens_ptr;
  std::vector<Light *> lights;
  BBox Worldbox;

public:
  // Constructors.
  World(); // initialize members.

  // Destructor.
  ~World(); // free memory.

  // Add to the scene.
  void add_geometry(Geometry *geom_ptr, bool is_wall=false);
  void add_light(Light *light_ptr);

  void set_camera(Camera *c_ptr);
  void set_lens(Lens *l_ptr);

  // Build scene - add all geometry, materials, lights, viewplane, camera,
  // samplers, and acceleration structures
  void build();

  // Returns appropriate shading information corresponding to intersection of
  // the ray with the scene geometry.
  ShadeInfo hit_objects(const Ray &ray, bool hit_walls=true);

  // Returns a float in [0, 1] corresponding to how much light hits the passed
  // hit_point.
  // For example: if 3/4 lights illuminate hit_point, return value will be 0.75.
  float get_light_value(const Point3D &hit_point, bool is_ray_primary=true);

  void addBBoxes();
};

