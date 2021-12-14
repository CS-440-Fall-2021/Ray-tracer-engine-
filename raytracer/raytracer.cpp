#include <iostream>
#include <chrono>

#include "materials/Cosine.hpp"

#include "samplers/Sampler.hpp"

#include "utilities/Image.hpp"
#include "utilities/RGBColor.hpp"
#include "utilities/Ray.hpp"
#include "utilities/ShadeInfo.hpp"

#include "world/World.hpp"
#include "world/ViewPlane.hpp"

#include "lenses/Lens.hpp"

#include "geometry/Plane.hpp"

auto start = std::chrono::high_resolution_clock::now();

// Simulation Parameters
const int NPR = 100; // Number of primary rays

int main(int argc, char **argv)
{
  World world;
  world.build();
  std::cout << "World built.\n";

  Sampler *sampler = world.sampler_ptr;
  ViewPlane &viewplane = world.vplane;
  Plane focal_plane(Point3D(0, 0, -15), Vector3D(0, 0, 1));
  Image image(viewplane);
  Lens lens(Point3D(0, 0, 0), Vector3D(0, 0, 1), 30.0);


  // std::cout << "Focal plane at: " + focal_plane.to_string() + "\n";

  for (int x = 0; x < viewplane.hres; x++) // across.
  {
    for (int y = 0; y < viewplane.vres; y++) // down.
    {
      
      Ray center_ray;
      std::vector<Ray> primary_rays;
      
      if (y % 100 == 0) {
        std::cout << "ViewPlane pixel: " + std::to_string(x) + ", " + std::to_string(y) + "\n";
      }
      
      RGBColor pixel_color(0);

      center_ray = sampler->get_center_ray(x, y);

      // std::cout << "Center Ray: " + center_ray.to_string() + "\n";

      float t = 0;
      ShadeInfo sinfo(world);
      bool hit = focal_plane.hit(center_ray, t, sinfo);

      // std::cout << "Hit: " + std::to_string(hit) + "\n";

      Point3D Pf = sinfo.hit_point;

      // std::cout << "Center ray hit point (Pf): " + Pf.to_string() + "\n";
      // std::cout << "Center ray hit point (t): " + std::to_string(t) + "\n";

      // Generate primary rays from Pi to Pf
      // where Pi is a random point on the lens

      // std::cout << "Generating Primary Rays...\n";
      for (int i = 0; i < NPR; i++)
      {
        Point3D origin = lens.get_random_point();
        Vector3D direction = (Pf - origin);

        Ray r(origin, direction);
        r.w = 1.0 / NPR;

        // if (i == 4) {
        //   std::cout << "Primary Ray #5:\n" + r.to_string() + "\n";
        // }

        primary_rays.push_back(r);
      }

      // std::cout << "Generated " + std::to_string(primary_rays.size()) + " primary rays\n";
      

      // std::cout << "Firing rays into the world...\n";
      for (const auto &ray : primary_rays)
      {
        float weight = ray.w; // ray weight for the pixel.
        ShadeInfo sinfo = world.hit_objects(ray);
        
        if (sinfo.hit)
        {
          // std::cout << "raytracer.cpp:99 - Primary ray hit at: " + sinfo.hit_point.to_string() + "\n";
          pixel_color += weight * sinfo.material_ptr->shade(sinfo);
        }
        else
        {
          pixel_color += weight * world.bg_color;
        }
        
      }
      // Save color to image.
      image.set_pixel(x, y, pixel_color);
    }
  }

  std::cout << "Raytracing complete.\n";

  // Write image to file.
  std::string filename = "scene_" + std::to_string(NPR) + "_uniform_random_r" + std::to_string((int)lens.radius) + ".ppm";
  image.write_ppm(filename);

  std::cout << "Wrote image.\n";

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "Execution time: " + std::to_string(duration.count()) << " milliseconds\n";

  return 0;
}
