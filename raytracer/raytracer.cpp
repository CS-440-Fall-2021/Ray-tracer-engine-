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
int NPR = 1000; // Number of primary rays
const bool blur = true;
int PROXIMITY_THRESHOLD = 20;
int main(int argc, char **argv)
{
  World world(PROXIMITY_THRESHOLD);
  world.build();
  std::cout << "World built.\n";

  Sampler *sampler = world.sampler_ptr;
  ViewPlane &viewplane = world.vplane;
  Image image(viewplane);
  Lens *lens = world.lens_ptr;
  Plane &focal_plane = world.lens_ptr->focal_plane;


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
      if (!blur) {
        NPR = 1;
      }
      
      for (int i = 0; i < NPR; i++)
      {
        Point3D origin;
        
        if (blur) {
          origin = lens->get_random_point();
        } else {
          origin = lens->origin;
        }

        Vector3D direction = (Pf - origin);

        Ray r(origin, direction);
        // r.w = (1.0 / NPR) * (lens.radius - (lens.origin.distance(origin))) / lens.radius;
        r.w = (1.0 / NPR);

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
          // TODO: cast shadow ray
          float light_val = world.get_light_value(sinfo.hit_point);

          pixel_color += light_val * weight * sinfo.material_ptr->shade(sinfo);
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
  float lens_to_vp = lens->origin.z - viewplane.top_left.z;
  std::string filename;
  if (blur) {
    filename = std::to_string(viewplane.hres) + "x" + std::to_string(viewplane.vres) + " - blur - " + std::to_string(NPR) + "NPR - URD - r" + std::to_string((int)lens->radius) + " - lens depth" +  std::to_string((int)lens_to_vp) + ".ppm";
  } else {
    filename = std::to_string(viewplane.hres) + "x" + std::to_string(viewplane.vres) + " - no blur - " + std::to_string(NPR) + "NPR - URD - r" + std::to_string((int)lens->radius) + " - lens depth" +  std::to_string((int)lens_to_vp) + ".ppm";
  }
  image.write_ppm(filename);

  std::cout << "Wrote image.\n";

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "Execution time: " + std::to_string(duration.count()) << " milliseconds\n";

  return 0;
}
