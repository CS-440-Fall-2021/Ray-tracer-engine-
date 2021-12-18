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
#include "../lenses/Lens.hpp"
#include "../utilities/BBox.hpp"

#include <map>
#include <iostream>
#include <algorithm>


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
  for (auto g : geometry) {
    free(g);
  }
  for (auto l : lights) {
    free(l);
  }
  free(camera_ptr);
  free(sampler_ptr);
  free(lens_ptr);
}

void World::add_geometry(Geometry* geom_ptr, bool is_wall) {
  if (is_wall) {
    walls.push_back(geom_ptr);
  }
  geometry.push_back(geom_ptr);
}

void World::add_light(Light* light_ptr) {
  lights.push_back(light_ptr);
}

void World::set_camera(Camera* c_ptr) {
  camera_ptr = c_ptr;
}

void World::set_lens(Lens* l_ptr) {
  lens_ptr = l_ptr;
}

void World::build() {
  // View plane  .
  vplane.top_left.x = -15;
  vplane.top_left.y = 15;
  vplane.top_left.z = -50;
  vplane.bottom_right.x = 15;
  vplane.bottom_right.y = -15;
  vplane.bottom_right.z = -50;
  vplane.hres = 400;
  vplane.vres = 400;

  // Background color.  
  bg_color = black;

  // Lens and sampler.
  Point3D lens_origin(0, 0, 0);
  Vector3D lens_normal(0, 0, 1);
  float lens_radius = 5.0;

  Point3D fp_origin(0, 0, -70);
  Vector3D fp_normal(0, 0, 1);

  Plane fp(fp_origin, fp_normal);

  set_lens(new Lens(lens_origin, lens_normal, lens_radius, fp));
  sampler_ptr = new Simple(lens_ptr, &vplane);

  // Lights
  Point3D light1_origin(-10, 20, -55);
  Vector3D light1_normal(0, -1, 0);
  float light1_fol = 90;

  Point3D light2_origin(10, 20, -70);
  Vector3D light2_normal(0, -1, 0);
  float light2_fol = 90;

  add_light(new Light(light1_origin, light1_normal, light1_fol));
  add_light(new Light(light2_origin, light2_normal, light2_fol));

  // Geometry
  Sphere* sphere_ptr = new Sphere(Point3D(-10, 0, -55), 5);
  sphere_ptr->set_material(new Cosine(red));
  add_geometry(sphere_ptr);

  Sphere* sphere_ptr_2 = new Sphere(Point3D(10, 0, -70), 5);
  sphere_ptr_2->set_material(new Cosine(blue));
  add_geometry(sphere_ptr_2);

  Plane* back = new Plane(Point3D(0, 0, -90), Vector3D(0, 0, 1));
  Plane* bottom = new Plane(Point3D(0, -10, 0), Vector3D(0, 1, 0));
  Plane* left = new Plane(Point3D(-20, 0, 0), Vector3D(1, 0, 0));
  Plane* right = new Plane(Point3D(20, 0, 0), Vector3D(-1, 0, 0));
  Plane* top = new Plane(Point3D(0, 21, 0), Vector3D(0, -1, 0));

  back->set_material(new Cosine(white));
  bottom->set_material(new Cosine(RGBColor(0.95, 0.95, 0.95)));
  left->set_material(new Cosine(white));
  right->set_material(new Cosine(white));
  top->set_material(new Cosine(white));

  add_geometry(back, true);
  add_geometry(bottom, true);
  add_geometry(left, true);
  add_geometry(right, true);
  add_geometry(top, true);

  // Point3D a(-5, 0, -55);
  // Point3D b(5, 0, -55);
  // Point3D c(0, 5, -55);
  // Triangle* tri_ptr = new Triangle(a, b, c);
  // tri_ptr->set_material(new Cosine(blue));
  // add_geometry(tri_ptr);
}


ShadeInfo World::hit_objects(const Ray& ray, bool hit_walls) {
  bool hit = false; // to keep track of whether a hit happened or not
  float t = 0; // represents a point on the ray
  ShadeInfo s(*this);

  float t_min = kHugeValue; // to keep track of the smallest t value
  ShadeInfo s_min(*this); // to keep track of the ShadeInfo associated with t_min


  for (auto geo_obj : geometry) {
    if (!hit_walls) {
      if (std::count(walls.begin(), walls.end(), geo_obj) != 0) {
        continue;
      }
    }

    hit = geo_obj->hit(ray, t, s);

    if (hit == true && t <= t_min) {
      t_min = t;
      s_min = ShadeInfo(s);
    }
  }
  return s_min;
}

float World::get_light_value(const Point3D &hit_point) {
  const int total_lights = lights.size();
  const float ind_light_weight = 1.0 / total_lights;
  float light_val = 0; 

  // For each light in the world, cast a shadow ray to it
  // and see if the ray reaches or not.
  for(Light *light : lights) {
    
    Vector3D dir = (light->origin - hit_point);
    dir.normalize();
    Ray shadow_ray(hit_point, dir);

    ShadeInfo sinfo = hit_objects(shadow_ray, false);

    if (sinfo.hit == false) {
      light_val += ind_light_weight;
    }
  }

  return light_val;
}