
/**
   This builds a scene that consists of 35 shaded spheres and a plane.
   Perspective viewing is used with a single sample per pixel.
   The spheres are the same as those in the Chapter 14 page one image. 

   Courtesy Kevin Suffern.
*/
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


void World::build() {
  //https://www.scratchapixel.com/lessons/advanced-rendering/introduction-acceleration-structure/grid
  
  
  // View plane
  vplane.top_left.x = -15;
  vplane.top_left.y = 15;
  vplane.top_left.z = -50;
  vplane.bottom_right.x = 15;
  vplane.bottom_right.y = -15;
  vplane.bottom_right.z = -50;
  vplane.hres = 360;
  vplane.vres = 360;

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
  Point3D light1_origin(0, 20, -55);
  Vector3D light1_normal(0, -1, 0);
  float light1_fol = 90;

  Point3D light2_origin(10, 20, -70);
  Vector3D light2_normal(0, -1, 0);
  float light2_fol = 90;

  add_light(new Light(light1_origin, light1_normal, light1_fol));
  add_light(new Light(light2_origin, light2_normal, light2_fol));

  // To store individual BBoxes of geometry in the scene
  std::vector<BBox*> BBoxes;
  
  
  // Geometry
  Sphere* sphere_ptr = new Sphere(Point3D(-10, 0, -55), 5);
  sphere_ptr->set_material(new Glossy(red));
  add_geometry(sphere_ptr);
  
  BBox* b1 = new BBox(sphere_ptr->getBBox());
  BBoxes.push_back(b1);


  Sphere* sphere_ptr_2 = new Sphere(Point3D(10, 0, -70), 5);
  sphere_ptr_2->set_material(new Glossy(blue));
  add_geometry(sphere_ptr_2);
  
  BBox* b2 = new BBox(sphere_ptr_2->getBBox());
  BBoxes.push_back(b2);

  Sphere* sphere_ptr_3 = new Sphere(Point3D(0, 7, -80), 5);
  sphere_ptr_3->set_material(new Glossy(green));
  add_geometry(sphere_ptr_3);
  
  BBox* b3 = new BBox(sphere_ptr_3->getBBox());
  BBoxes.push_back(b3);

  Sphere* sphere_ptr_4 = new Sphere(Point3D(3, -3, -55), 5);
  sphere_ptr_4->set_material(new Glossy(RGBColor(0.88, 0.67, 0)));
  add_geometry(sphere_ptr_4);
  
  BBox* b4 = new BBox(sphere_ptr_4->getBBox());
  BBoxes.push_back(b4);

  // Plane* back = new Plane(Point3D(0, 0, -90), Vector3D(0, 0, 1));
  Plane* bottom = new Plane(Point3D(0, -10, 0), Vector3D(0, 1, 0));
  // Plane* left = new Plane(Point3D(-20, 0, 0), Vector3D(1, 0, 0));
  // Plane* right = new Plane(Point3D(20, 0, 0), Vector3D(-1, 0, 0));
  // Plane* top = new Plane(Point3D(0, 21, 0), Vector3D(0, -1, 0));

  // back->set_material(new Wall(white));
  bottom->set_material(new Wall(RGBColor(0.7, 0.7, 0.7)));
  // left->set_material(new Wall(RGBColor(0.9, 0.9, 0.9)));
  // right->set_material(new Wall(RGBColor(0.9, 0.9, 0.9)));
  // top->set_material(new Wall(white));

  // add_geometry(back, true);
  add_geometry(bottom, true);
  // add_geometry(left, true);
  // add_geometry(right, true);
  // add_geometry(top, true);


  this->Worldbox = BBox::extend(BBoxes);
  this->Worldbox.pmin.z -= 10;

  
  this->acceleration.init(this->geometry, this->walls, this->Worldbox);
  this->acceleration.generateGrid();
}