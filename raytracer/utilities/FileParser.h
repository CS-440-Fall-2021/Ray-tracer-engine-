//
// Created by Owais Bin Asad on 10/09/2025.
//

#ifndef RAY_TRACER_ENGINE__FILEPARSER_H
#define RAY_TRACER_ENGINE__FILEPARSER_H
#include <string>
#include <vector>

#include "Geometry.hpp"
#include "Light.hpp"

void buildWorldFromFile(const std::string &filename, std::vector<Geometry *>& geometry, std::vector<BBox*>& bboxes, std::vector<Light *>& lights, std::vector<Geometry *>& walls);
void buildAndPushSphere(const std::vector<std::string> &tokens, std::vector<Geometry *> &geometry,
                        std::vector<BBox *> &bboxes);
void buildAndPushLight(const std::vector<std::string> &tokens, std::vector<Light *>& lights,
                        std::vector<BBox *> &bboxes);
void buildAndPushWall(const std::vector<std::string> &tokens, std::vector<Geometry *>& walls, std::vector<Geometry *> &geometry);
#endif //RAY_TRACER_ENGINE__FILEPARSER_H