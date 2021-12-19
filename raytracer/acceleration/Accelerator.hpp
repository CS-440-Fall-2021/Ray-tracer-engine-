#pragma once

#include "../utilities/BBox.hpp"
#include "../utilities/Vector3D.hpp"
#include "../utilities/ShadeInfo.hpp"
#include "../geometry/Geometry.hpp"

class Accelerator {
public:
    BBox Worldbox;
    std::vector<BBox*> BBoxes;
    Vector3D resolution;
    Vector3D celldim;
    std::vector<Geometry *>* Grid;
    int num_rows;
    std::vector<Geometry *> *geometry;
    std::vector<Geometry *> *walls;

    Accelerator() = default;
    ~Accelerator() = default;

    // Takes all the geometry in the scene and creates individual BBoxes
    // as well as the Worldbox.
    void init(std::vector<Geometry*> &geometry, std::vector<Geometry*> &walls, BBox &Worldbox);
    
    // Generates the grid and populates the Grid arary with elements.
    void generateGrid();

    // Prints out the grid for debugging.
    void printGrid();

    ShadeInfo hit_objects(const Ray &ray, World& wr, bool hit_walls=true);
};