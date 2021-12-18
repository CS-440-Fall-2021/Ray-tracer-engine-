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
#include <cmath>

World::World(float f) {
  // Feel free to override in World::build() as needed.
  PROXIMITY_THRESHOLD = f;
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

  std::vector<BBox*> BBoxes; 
  // Geometry
  Sphere* sphere_ptr = new Sphere(Point3D(-10, 0, -55), 5);
  sphere_ptr->set_material(new Cosine(red));
  add_geometry(sphere_ptr);
  BBox* b1 = new BBox(sphere_ptr->getBBox());
  BBoxes.push_back(b1);


  Sphere* sphere_ptr_2 = new Sphere(Point3D(10, 0, -70), 5);
  sphere_ptr_2->set_material(new Cosine(blue));
  add_geometry(sphere_ptr_2);
  BBox* b2 = new BBox(sphere_ptr_2->getBBox());
  BBoxes.push_back(b2);

  Plane* back = new Plane(Point3D(0, 0, -90), Vector3D(0, 0, 1));
  Plane* bottom = new Plane(Point3D(0, -10, 0), Vector3D(0, 1, 0));

  back->set_material(new Cosine(white));
  bottom->set_material(new Cosine(RGBColor(0.75, 0.75, 0.75)));

  add_geometry(back, true);
  BBox* b3 = new BBox(back->getBBox());
  BBoxes.push_back(b3);

  add_geometry(bottom, true);
  BBox* b4 = new BBox(bottom->getBBox());
  BBoxes.push_back(b4);

  Point3D a(-5, 0, -55);
  Point3D b(5, 0, -55);
  Point3D c(0, 5, -55);
  Triangle* tri_ptr = new Triangle(a, b, c);
  tri_ptr->set_material(new Cosine(blue));
  add_geometry(tri_ptr);
  BBox* b5 = new BBox(tri_ptr->getBBox());
  BBoxes.push_back(b5);

  this->Worldbox = BBox::extend(BBoxes);
  
  Vector3D size_of_world = this->Worldbox.pmax - this->Worldbox.pmin;
  
  int Total_primitives = geometry.size();
  float Croot = std::pow(5*Total_primitives/(size_of_world.x * size_of_world.y *size_of_world.z), 1/3);

  Vector3D Resolution = Point3D(
    std::floor(size_of_world.x* Croot),
    std::floor(size_of_world.y* Croot),
    std::floor(size_of_world.z* Croot)
  );

  Resolution = Point3D(
    std::max(double(1),std::min(Resolution.x,double(128))),
    std::max(double(1),std::min(Resolution.y,double(128))),
    std::max(double(1),std::min(Resolution.z,double(128)))
  );
  
  Vector3D celldim = Vector3D(
    size_of_world.x / Resolution.x ,
    size_of_world.y / Resolution.y ,
    size_of_world.z / Resolution.z 
  );
  
  this->num_rows = Resolution.x * Resolution.y * Resolution.z;

  this->Grid = new std::vector<Geometry *>[this->num_rows];

  for (auto geo : geometry){
    
    BBox temp = geo->getBBox();
    
    Point3D min = temp.pmin;
    Point3D max = temp.pmax;
    
    min = Point3D(
      (min.x-this->Worldbox.pmin.x)/celldim.x,
      (min.y-this->Worldbox.pmin.y)/celldim.y,
      (min.z- this->Worldbox.pmin.z)/celldim.z
    );
    
    max = Point3D(
      (max.x-this->Worldbox.pmin.x)/celldim.x,
      (max.y-this->Worldbox.pmin.y)/celldim.y,
      (max.z- this->Worldbox.pmin.z)/celldim.z
    );
    
  
    int zmin = std::min(std::max(std::floor(min.z),float(0)),float(Resolution.z -1));
    int zmax = std::min(std::max(std::floor(max.z),float(0)),float(Resolution.z -1));
    int ymin = std::min(std::max(std::floor(min.y),float(0)),float(Resolution.y -1));
    int ymax = std::min(std::max(std::floor(max.y),float(0)),float(Resolution.y -1));
    int xmin = std::min(std::max(std::floor(min.x),float(0)),float(Resolution.x -1));
    int xmax = std::min(std::max(std::floor(max.x),float(0)),float(Resolution.x -1));
    

    for (int z = zmin; z<=zmax; ++z){
      for(int y= ymin; y<=ymax; ++y){
        for(int x= xmin; x<=xmax; ++x){

          int pos = z * Resolution.x * Resolution.y + y * Resolution.x + x;
          std::cout << "pos is:" +std::to_string(pos)+"\n";
          this->Grid[pos] .push_back(geo);
          std::cout<< "pushed\n";
        }
      }
    }    
  }

    for (int i=0; i< num_rows; i= i +1){
      for (auto geo_obj : this->Grid[i]){
        if (geo_obj != NULL){
          std::cout<< "A Shape in row: "+ std::to_string(i) + "is " + geo_obj->to_string()+ "\n";
        }
      }
    }
}

// void World::addBBoxes(){
//   std::vector<BBox*> BBoxes; 
//   for (auto geo_obj : geometry){
//     std::cout<< "Geometry is :" + geo_obj->to_string()+"\n";
//     BBox temporary = geo_obj->getBBox();
//     std::cout<< "BBox is :" + temporary.to_string()+"\n";
    
//     BBoxes.push_back(&temporary);
//   }
  
//   this->Worldbox = BBox::extend(BBoxes);

// }

// void World::cluster(){
//   std::vector<BBox *> result;
//   std::vector<BBox *> done;
//   std::cout<< "Worldbox.children.size() :" + std::to_string(Worldbox.children.size())+ "\n";
//   for (auto current : this->Worldbox.children){
//     bool flag = false;
//     for(auto check : this->Worldbox.children){
//       if(std::count(done.begin(), done.end(), current) > 0 ){
//         break;
//       }

//       if((std::count(done.begin(), done.end(), check) > 0)||
//         (current->to_string() == check->to_string())){
//         flag = true;
//         continue;
//       }
//       std::cout<<"KUCH";
//       Point3D midcurrent = Point3D((current->pmin.x + current->pmax.x)* 0.5,(current->pmin.y + current->pmax.y)* 0.5,(current->pmin.z + current->pmax.z)* 0.5);
//       Point3D midcheck = Point3D((check->pmin.x + check->pmax.x)* 0.5,(check->pmin.y + check->pmax.y)* 0.5,(check->pmin.z + check->pmax.z)* 0.5);

//       float distance = (midcheck-midcurrent).length();
//       if (distance <= PROXIMITY_THRESHOLD){
//         BBox temp = current->extend(*check);
//         result.push_back(&temp);
//         done.push_back(current);
//         done.push_back(check);
//         flag = true;
//       }

//     }
//     if (flag == false){
//       done.push_back(current);
//       result.push_back(current);
//     }
//     else{
//       done.push_back(current);
//     }
//   }
//   std::cout<< "result.size(): "+ std::to_string(result.size())+ "\n";
//   this->Worldbox.children = result;
// }

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