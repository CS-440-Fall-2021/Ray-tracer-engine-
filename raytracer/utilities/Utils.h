//
// Created by Owais Bin Asad on 10/09/2025.
//

#ifndef RAY_TRACER_ENGINE__UTILS_H
#define RAY_TRACER_ENGINE__UTILS_H
#include <string>
#include <vector>

std::string stringify(int input);
std::string stringify(long long input);
std::string stringify(bool input);
std::string getCurrentDateTime();
std::string generateFileName(int hres, int vres, bool blur, int NPR, bool secRays, float lensRadius, float lensDepth);
std::vector<std::string> split(const std::string& s, char delimiter);

#endif //RAY_TRACER_ENGINE__UTILS_H
