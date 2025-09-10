#include "World.hpp"
#include "../cameras/Perspective.hpp"
#include "../cameras/Parallel.hpp"
#include "../geometry/Plane.hpp"
#include "../geometry/Sphere.hpp"
#include "../geometry/Triangle.hpp"
#include "../materials/Cosine.hpp"
#include "../materials/Wall.hpp"
#include "../materials/Matte.hpp"
#include "../materials/Glossy.hpp"
#include "../samplers/Simple.hpp"
#include "../utilities/Constants.hpp"
#include "../utilities/ShadeInfo.hpp"
#include "../lenses/Lens.hpp"
#include "../utilities/BBox.hpp"
#include "../acceleration/Accelerator.hpp"
#include <map>
#include <iostream>
#include <algorithm>
#include <cmath>

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

  for (const auto bbox : bboxes) {
    free(bbox);
  }
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


ShadeInfo World::hit_objects(const Ray& ray, bool hit_walls) {
  if (to_accelerate) {
    return this->acceleration.hit_objects(ray, *this, hit_walls);
  }
  else {
    return unaccel_hit_objects(ray, hit_walls);
  }
}

ShadeInfo World::unaccel_hit_objects(const Ray& ray, bool hit_walls) {
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

float World::get_light_value(const Point3D &hit_point, const Vector3D &normal) {
  const int total_lights = static_cast<int>(lights.size());
  const auto ind_light_weight = static_cast<float>(1.0 / total_lights);
  float light_val = 0;

  // For each light in the world, cast a shadow ray to it
  // and see if the ray reaches or not.
  for (const Light* light : lights) {

    Vector3D dir = light->origin - hit_point;
    const auto distance = static_cast<float>(dir.length());
    dir.normalize();

    Point3D shadow_ray_origin = hit_point + normal * kEpsilon;
    Ray shadow_ray(shadow_ray_origin, dir);

    // If it doesn't hit anything, that means the shadow ray is able to reach the light
    if (hit_objects(shadow_ray).hit == false) {
      const float angle = static_cast<float>(abs(acos(-dir * light->normal) * 180 / PI));

      if (angle <= light->fol) {
        light_val += static_cast<float>(500.0 / pow(distance, 2) * ind_light_weight);
      }
    }
  }

  return light_val;
}
