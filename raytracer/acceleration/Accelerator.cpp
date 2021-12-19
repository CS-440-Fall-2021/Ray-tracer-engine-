#include "./Accelerator.hpp"
#include <algorithm>
#include <cmath>

void Accelerator::init(std::vector<Geometry*>& geometry, std::vector<Geometry*>& walls, BBox &Worldbox) {
    
    this->geometry = &geometry;
    this->walls = &walls;

    // for (auto geo : geometry) {
    //     if (std::count(walls.begin(), walls.end(), geo) != 0) {
    //         continue;
    //     }
    //     BBoxes.push_back(new BBox(geo->getBBox()));
    // }

    this->Worldbox = Worldbox;

    // // TODO
    // this->Worldbox.pmin.z -= 10;
}

void Accelerator::generateGrid() {
    Vector3D size_of_world = this->Worldbox.pmax - this->Worldbox.pmin;

    int Total_primitives = geometry->size();

    float Croot = std::cbrt(Total_primitives / (size_of_world.x * size_of_world.y * size_of_world.z));

    this->resolution = Point3D(
        std::floor(size_of_world.x * Croot),
        std::floor(size_of_world.y * Croot),
        std::floor(size_of_world.z * Croot)
    );

    this->resolution = Point3D(
        std::max(double(1), std::min(this->resolution.x, double(128))),
        std::max(double(1), std::min(this->resolution.y, double(128))),
        std::max(double(1), std::min(this->resolution.z, double(128)))
    );

    this->celldim = Vector3D(
        size_of_world.x / this->resolution.x,
        size_of_world.y / this->resolution.y,
        size_of_world.z / this->resolution.z
    );

    this->num_rows = this->resolution.x * this->resolution.y * this->resolution.z;

    this->Grid = new std::vector<Geometry*>[this->num_rows];

    for (auto geo : *geometry) {

        if (std::count(walls->begin(), walls->end(), geo) != 0) {
            continue;
        }
        if (geo != NULL) {
            BBox temp = geo->getBBox();

            Point3D min = temp.pmin;
            Point3D max = temp.pmax;

            min = Point3D(
                (min.x - this->Worldbox.pmin.x) / this->celldim.x,
                (min.y - this->Worldbox.pmin.y) / this->celldim.y,
                (min.z - this->Worldbox.pmin.z) / this->celldim.z
            );

            max = Point3D(
                (max.x - this->Worldbox.pmin.x) / this->celldim.x,
                (max.y - this->Worldbox.pmin.y) / this->celldim.y,
                (max.z - this->Worldbox.pmin.z) / this->celldim.z
            );


            int zmin = std::min(std::max(std::floor(min.z), float(0)), float(this->resolution.z - 1));
            int zmax = std::min(std::max(std::floor(max.z), float(0)), float(this->resolution.z - 1));
            int ymin = std::min(std::max(std::floor(min.y), float(0)), float(this->resolution.y - 1));
            int ymax = std::min(std::max(std::floor(max.y), float(0)), float(this->resolution.y - 1));
            int xmin = std::min(std::max(std::floor(min.x), float(0)), float(this->resolution.x - 1));
            int xmax = std::min(std::max(std::floor(max.x), float(0)), float(this->resolution.x - 1));


            for (int z = zmin; z <= zmax; ++z) {
                for (int y = ymin; y <= ymax; ++y) {
                    for (int x = xmin; x <= xmax; ++x) {
                        int pos = (z * this->resolution.x * this->resolution.y) + (y * this->resolution.x) + x;
                        this->Grid[pos].push_back(geo);
                    }
                }
            }
        }
    }
}

void Accelerator::printGrid() {
    std::cout << std::to_string(num_rows);

    for (int i = 0; i < num_rows; i++) {
        for (auto geo_obj : this->Grid[i]) {
            if (geo_obj != NULL) {
                std::cout << "A Shape in row: " + std::to_string(i) + "is " + geo_obj->to_string() + "\n";
            }
        }
    }
}

ShadeInfo Accelerator::hit_objects(const Ray& ray, World& wr,bool hit_walls ) {
    //https://www.scratchapixel.com/lessons/advanced-rendering/introduction-acceleration-structure/grid

    bool hit = false; // to keep track of whether a hit happened or not
    float t = 0; // represents a point on the ray
    ShadeInfo s(wr);

    float t_min = kHugeValue; // to keep track of the smallest t value
    ShadeInfo s_min(wr); // to keep track of the ShadeInfo associated with t_min
    float t_enter;
    float t_exit;

    float t_min_wall = kHugeValue;
    ShadeInfo s_min_wall(wr);

    if (hit_walls) {
        for (auto wall : *this->walls) {
            hit = wall->hit(ray, t, s);

            if (hit == true && t < t_min) {
                t_min_wall = t;
                s_min_wall = ShadeInfo(s);
            }
        }
    }


    if (Worldbox.hit(ray, t_enter, t_exit) == false) {
        return s_min_wall;
    }


    Vector3D exit_int, step_int;
    Vector3D deltaT, nextCrossingT;

    Point3D rayOrigCell = Point3D(
        ((ray.o.x + ray.d.x * t_enter) - Worldbox.pmin.x),
        ((ray.o.x + ray.d.x * t_enter) - Worldbox.pmin.y),
        ((ray.o.x + ray.d.x * t_enter) - Worldbox.pmin.z));

    Point3D cell = Point3D(
        std::min(std::max(std::floor(rayOrigCell.x / this->celldim.x), double(0.0)), this->resolution.x - 1),
        std::min(std::max(std::floor(rayOrigCell.y / this->celldim.y), double(0.0)), this->resolution.y - 1),
        std::min(std::max(std::floor(rayOrigCell.z / this->celldim.z), double(0.0)), this->resolution.z - 1));


    if (ray.d.x < 0) {
        deltaT.x = -this->celldim.x / (ray.d.x);
        nextCrossingT.x = t_enter + (cell.x * this->celldim.x - rayOrigCell.x) / (ray.d.x);
        exit_int.x = -1;
        step_int.x = -1;
    }   else {
        deltaT.x = this->celldim.x / (ray.d.x);
        nextCrossingT.x = t_enter + ((cell.x + 1) * this->celldim.x - rayOrigCell.x) / (ray.d.x);
        exit_int.x = this->resolution.x;
        step_int.x = 1;
    }

    if (ray.d.y < 0) {

        deltaT.y = -this->celldim.y / (ray.d.y);
        nextCrossingT.y = t_enter + (cell.y * this->celldim.y - rayOrigCell.y) / (ray.d.y);
        exit_int.y = -1;
        step_int.y = -1;
    }   else {

        deltaT.y = this->celldim.y / (ray.d.y);
        nextCrossingT.y = t_enter + ((cell.y + 1) * this->celldim.y - rayOrigCell.y) / (ray.d.y);
        exit_int.y = this->resolution.y;
        step_int.y = 1;
    }

    if (ray.d.z < 0) {

        deltaT.z = -this->celldim.z / (ray.d.z);
        nextCrossingT.z = t_enter + (cell.z * this->celldim.z - rayOrigCell.z) / (ray.d.z);
        exit_int.z = -1;
        step_int.z = -1;
    }   else {

        deltaT.z = this->celldim.z / (ray.d.z);
        nextCrossingT.z = t_enter + ((cell.z + 1) * this->celldim.z - rayOrigCell.z) / (ray.d.z);
        exit_int.z = this->resolution.z;
        step_int.z = 1;
    }


    while (1) {
        int c = cell.z * this->resolution.x * this->resolution.y +
            cell.y * this->resolution.x +
            cell.x;

        for (auto geo_obj : this->Grid[c]) {
            if (geo_obj != NULL) {
                if (!hit_walls) {
                    if (std::count(walls->begin(), walls->end(), geo_obj) != 0) {
                        continue;
                    }
                }

                hit = geo_obj->hit(ray, t, s);

                if (hit == true && t < t_min) {
                    t_min = t;
                    s_min = ShadeInfo(s);
                }
            }
        }
        int k = (int(nextCrossingT.x < nextCrossingT.y) << 2 +
            int(nextCrossingT.x < nextCrossingT.z) << 1 +
            int(nextCrossingT.y < nextCrossingT.z));


        static const int mapping[8] = { 2,1,2,1,2,2,0,0 };
        int axis = mapping[k];

        if (axis == 0) {
            if (double(t_min) < nextCrossingT.x) break;
            cell.x += step_int.x;
            if (cell.x == exit_int.x) break;
            nextCrossingT.x += deltaT.x;
        }     else if (axis == 1) {
            if (double(t_min) < nextCrossingT.y) break;
            cell.y += step_int.y;
            if (cell.y == exit_int.y) break;
            nextCrossingT.y += deltaT.y;
        }     else {
            if (double(t_min) < nextCrossingT.z) break;
            cell.z += step_int.z;
            if (cell.z == exit_int.z) break;
            nextCrossingT.z += deltaT.z;
        }
    }
    if (t_min < t_min_wall) {

        return s_min;
    }   else {
        return s_min_wall;
    }
}