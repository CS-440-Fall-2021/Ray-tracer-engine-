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
#include "utilities/Utils.h"
#include <omp.h>

auto start = std::chrono::high_resolution_clock::now();

float getLightingIntensity(const ShadeInfo& primary_ray_sinfo, World& world) {
  if constexpr (!lighting) return 1;
  return world.get_light_value(primary_ray_sinfo.hit_point, primary_ray_sinfo.normal);
}

void castSecondaryRays(const Ray& incident_ray, const ShadeInfo& incident_ray_sinfo, World& world, const float incident_ray_weight, RGBColor& pixel_color) {
  if constexpr (!secondary_rays) return;

  // Determine direction of secondary ray
  const Vector3D a = (incident_ray.d)*(incident_ray_sinfo.normal);
  const Vector3D b = 2*((a)*(incident_ray_sinfo.normal));
  Vector3D sec_ray_dir = incident_ray.d - b;
  sec_ray_dir.normalize();

  // Cast secondary ray
  const Point3D incident_ray_hit_point = incident_ray_sinfo.hit_point + incident_ray_sinfo.normal * kEpsilon;
  const auto sec_ray = Ray(incident_ray_hit_point, sec_ray_dir);
  const ShadeInfo sec_ray_sinfo = world.hit_objects(sec_ray);

  RGBColor sec_ray_col;

  if (sec_ray_sinfo.hit)
  {
    // The secondary ray hit something
    const float light_intensity = getLightingIntensity(sec_ray_sinfo, world);
    sec_ray_col = sec_ray_sinfo.material_ptr->shade(sec_ray_sinfo) * light_intensity;
  }
  else
  {
    sec_ray_col = world.bg_color;
  }
  pixel_color += incident_ray_weight * brightness_adjustment * incident_ray_sinfo.material_ptr->get_r_index() * sec_ray_col;
}

void generatePrimaryRays(const int x, const int y, const Sampler *sampler, const World& world, const Plane& focal_plane, const int _NPR, Lens* lens, std::vector<Ray>& primary_rays) {
  const Ray center_ray = sampler->get_center_ray(x, y);

  float t = 0;
  ShadeInfo center_ray_sinfo(world);
  focal_plane.hit(center_ray, t, center_ray_sinfo);

  // Pf = The point where the center ray hits the focal plane
  const Point3D Pf = center_ray_sinfo.hit_point;

  // Generate primary rays
  // from the lens to the Pf
  for (int i = 0; i < _NPR; i++)
  {
    Point3D origin = blur ? lens->get_random_point() : lens->origin;
    Vector3D direction = Pf - origin;
    Ray r(origin, direction);

    r.w = static_cast<float>(1.0 / _NPR);

    primary_rays.push_back(r);
  }
}

int main(int argc, char **argv)
{
  World world;
  world.build();
  std::cout << "World built.\n";

  Sampler *sampler = world.sampler_ptr;
  ViewPlane &viewplane = world.vplane;
  Image image(viewplane);
  Lens *lens = world.lens_ptr;
  Plane &focal_plane = world.lens_ptr->focal_plane;

  int totalProcessedColumns = 0;
  constexpr int _NPR = blur ? NPR : 1;

  #pragma omp parallel for
  for (int x = 0; x < viewplane.hres; x++)
  {
    for (int y = 0; y < viewplane.vres; y++)
    {
      std::vector<Ray> primary_rays;
      RGBColor pixel_color(0);

      generatePrimaryRays(x, y, sampler, world, focal_plane, _NPR, lens, primary_rays);

      for (const auto &primary_ray : primary_rays)
      {
        float primary_ray_weight = primary_ray.w;
        ShadeInfo primary_ray_sinfo = world.hit_objects(primary_ray);
        
        // The primary ray hit something
        if (primary_ray_sinfo.hit)
        {
          RGBColor primary_color = primary_ray_sinfo.material_ptr->shade(primary_ray_sinfo);
          float light_intensity = getLightingIntensity(primary_ray_sinfo, world);

          pixel_color += primary_ray_weight * brightness_adjustment * primary_color * light_intensity;

          castSecondaryRays(primary_ray, primary_ray_sinfo, world, primary_ray_weight, pixel_color);
        }
        else
        {
          // The primary ray did not hit anything
          pixel_color += primary_ray_weight * world.bg_color;
        }
      }
      // Save color to image.
      image.set_pixel(x, y, pixel_color);
    }
    totalProcessedColumns += 1;
    std::cout << "Total processed columns: " << totalProcessedColumns << "\n";
  }

  std::cout << "Raytracing complete.\n";

  // Write image to file.
  float lens_to_vp = lens->origin.z - viewplane.top_left.z;
  std::string filename = generateFileName(
    viewplane.hres,
    viewplane.vres,
    blur,
    _NPR,
    secondary_rays,
    lens->radius,
    lens_to_vp
  );
  image.write_ppm(filename);

  std::cout << "Wrote image.\n";

  auto stop = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

  std::cout << "Execution time: " + std::to_string(duration.count()) << " milliseconds\n";

  return 0;
}
