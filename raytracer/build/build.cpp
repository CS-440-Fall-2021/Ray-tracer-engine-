/**
   This builds a simple scene that consists of a sphere, a triangle, and a
   plane.
   Parallel viewing is used with a single sample per pixel.
*/
#include "FileParser.h"
#include "World.hpp"
#include "../acceleration/Accelerator.hpp"
#include "../cameras/Parallel.hpp"
#include "../cameras/Perspective.hpp"
#include "../geometry/Plane.hpp"
#include "../lenses/Lens.hpp"
#include "../materials/Cosine.hpp"
#include "../samplers/Simple.hpp"
#include "../utilities/BBox.hpp"
#include "../utilities/Constants.hpp"

//https://www.scratchapixel.com/lessons/advanced-rendering/introduction-acceleration-structure/grid

void World::build() {
    // View plane
    vplane.top_left.x = -15;
    vplane.top_left.y = 15;
    vplane.top_left.z = -30;
    vplane.bottom_right.x = 15;
    vplane.bottom_right.y = -15;
    vplane.bottom_right.z = -30;
    vplane.hres = 500;
    vplane.vres = 500;

    // Background color.  
    bg_color = black;

    // Focal Plane
    const Point3D focal_plane_origin(0, 0, -45);
    const Vector3D focal_plane_normal(0, 0, 1);
    const Plane focal_plane(focal_plane_origin, focal_plane_normal);

    // Lens
    const Point3D lens_origin(0, 0, 0);
    const Vector3D lens_normal(0, 0, 1);
    constexpr float lens_radius = 1;

    set_lens(new Lens(lens_origin, lens_normal, lens_radius, focal_plane));

    // Sampler
    sampler_ptr = new Simple(lens_ptr, &vplane);


    const auto filename = std::string("../build/scene.txt");
    buildWorldFromFile(filename, geometry, bboxes, lights, walls);


    this->worldbox = BBox::extend(bboxes);
    this->worldbox.pmin.z -= 10;


    this->acceleration.init(geometry, walls, worldbox);
    this->acceleration.generateGrid();
}
