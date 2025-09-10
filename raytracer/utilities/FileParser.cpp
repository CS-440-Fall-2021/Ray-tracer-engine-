//
// Created by Owais Bin Asad on 10/09/2025.
//

#include "FileParser.h"

#include <iostream>
#include <ostream>

#include "BBox.hpp"
#include "Constants.hpp"
#include "FileReader.h"
#include "Glossy.hpp"
#include "Light.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "Utils.h"
#include "Vector3D.hpp"
#include "Wall.hpp"

void buildWorldFromFile(const std::string &filename, std::vector<Geometry *> &geometry, std::vector<BBox *> &bboxes,
                        std::vector<Light *> &lights, std::vector<Geometry *> &walls) {
    const auto lines = getLinesFromFile(filename);

    for (const auto &line: lines) {
        auto tokens = split(line, ' ');

        const std::string &object = tokens[0];

        if (object == "sphere") {
            buildAndPushSphere(tokens, geometry, bboxes);
        } else if (object == "light") {
            buildAndPushLight(tokens, lights, bboxes);
        } else if (object == "wall") {
            buildAndPushWall(tokens, walls, geometry);
        } else {
            std::cerr << "Unknown object: " << object << std::endl;
        }
    }
}

void buildAndPushSphere(const std::vector<std::string> &tokens, std::vector<Geometry *> &geometry,
                        std::vector<BBox *> &bboxes) {
    const float origin_x = std::stof(tokens[1]);
    const float origin_y = std::stof(tokens[2]);
    const float origin_z = std::stof(tokens[3]);

    const float radius = std::stof(tokens[4]);

    const std::string &material = tokens[5];

    const float color_r = std::stof(tokens[6]);
    const float color_g = std::stof(tokens[7]);
    const float color_b = std::stof(tokens[8]);

    Point3D origin(origin_x, origin_y, origin_z);
    auto *sphere = new Sphere(origin, radius);

    if (material == "glossy") {
        auto *glossy = new Glossy(color_r, color_g, color_b);
        sphere->set_material(glossy);
    } else {
        std::cerr << "Unkown material: " << material << std::endl;
    }

    geometry.push_back(sphere);

    const auto bb = new BBox(sphere->getBBox());
    bboxes.push_back(bb);
}

void buildAndPushLight(const std::vector<std::string> &tokens, std::vector<Light *> &lights,
                       std::vector<BBox *> &bboxes) {
    const float origin_x = std::stof(tokens[1]);
    const float origin_y = std::stof(tokens[2]);
    const float origin_z = std::stof(tokens[3]);

    const float normal_x = std::stof(tokens[4]);
    const float normal_y = std::stof(tokens[5]);
    const float normal_z = std::stof(tokens[6]);

    const float fol = std::stof(tokens[7]);

    const Point3D origin(origin_x, origin_y, origin_z);
    const Vector3D normal(normal_x, normal_y, normal_z);

    const auto light = new Light(origin, normal, fol);

    lights.push_back(light);
}

void buildAndPushWall(const std::vector<std::string> &tokens, std::vector<Geometry *> &walls, std::vector<Geometry *> &geometry) {
    const float origin_x = std::stof(tokens[1]);
    const float origin_y = std::stof(tokens[2]);
    const float origin_z = std::stof(tokens[3]);

    const float normal_x = std::stof(tokens[4]);
    const float normal_y = std::stof(tokens[5]);
    const float normal_z = std::stof(tokens[6]);

    const float color_r = std::stof(tokens[7]);
    const float color_g = std::stof(tokens[8]);
    const float color_b = std::stof(tokens[9]);

    const Point3D origin(origin_x, origin_y, origin_z);
    const Vector3D normal(normal_x, normal_y, normal_z);

    const auto wall = new Plane(origin, normal);
    wall->set_material(new Wall(color_r, color_g, color_b));

    walls.push_back(wall);
    geometry.push_back(wall);
}
