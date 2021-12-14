#include "World.hpp"
#include "../cameras/Perspective.hpp"
#include "../cameras/Parallel.hpp"
#include "../geometry/Plane.hpp"
#include "../geometry/Sphere.hpp"
#include "../geometry/Triangle.hpp"
#include "../materials/Cosine.hpp"
#include "../samplers/Simple.hpp"
#include "../utilities/Constants.hpp"
#include "../utilities/ShadeInfo.hpp"

#include <map>
#include <iostream>


World::World() {
  // Feel free to override in World::build() as needed.
  vplane.set_hres(640);
  vplane.set_vres(480);
  vplane.top_left = Point3D(-320, 240, 0);
  vplane.bottom_right = Point3D(320, -240, 0);
  vplane.normal = Vector3D(0, 0, -1);

  bg_color = RGBColor(0, 0, 0);
}

World::~World() {
  for (auto g : geometry)
  {
    free(g);
  }
  free(camera_ptr);
  free(sampler_ptr);
}

void World::add_geometry(Geometry *geom_ptr) {
  geometry.push_back(geom_ptr);
}

void World::set_camera(Camera *c_ptr) {
  camera_ptr = c_ptr;
}

void World::build() {
  // View plane  .
  vplane.top_left.x = -15;
  vplane.top_left.y = 15;
  vplane.top_left.z = -10;
  vplane.bottom_right.x = 15;
  vplane.bottom_right.y = -15;
  vplane.bottom_right.z = -10;
  vplane.hres = 400;
  vplane.vres = 400;

  // Background color.  
  bg_color = black;
  
  // Camera and sampler.
  set_camera(new Perspective(0, 0, 20));
  sampler_ptr = new Simple(camera_ptr, &vplane);
	
  // sphere
  Sphere* sphere_ptr = new Sphere(Point3D(0, 0, -15), 5);
  sphere_ptr->set_material(new Cosine(red));
  add_geometry(sphere_ptr);

  Sphere* sphere_ptr_2 = new Sphere(Point3D(12, 12, -15), 5);
  sphere_ptr_2->set_material(new Cosine(blue));
  add_geometry(sphere_ptr_2);
}

ShadeInfo World::hit_objects(const Ray &ray) {
  bool hit = false; // to keep track of whether a hit happened or not
  float t = 0; // represents a point on the ray
  ShadeInfo s(*this);

  float t_min = kHugeValue; // to keep track of the smallest t value
  ShadeInfo s_min(*this); // to keep track of the ShadeInfo associated with t_min
  

  for (auto geo_obj : geometry)
  {
    hit = geo_obj->hit(ray, t, s);
    
    if (hit == true && t <= t_min) {
      t_min = t;
      s_min = ShadeInfo(s);
    }
  }
  return s_min;
}