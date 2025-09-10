//
// Created by Owais Bin Asad on 10/09/2025.
//

#include <string>
#include "Utils.h"
#include <sstream>
#include <iomanip>

std::string stringify(const int input) {
    return std::to_string(input);
}

std::string stringify(const long long input) {
    return std::to_string(input);
}

std::string stringify(const bool input) {
    return input ? "true" : "false";
}

std::string getCurrentDateTime() {
    // Get current time as time_point
    auto now = std::chrono::system_clock::now();

    // Convert to time_t for C-style formatting
    std::time_t time = std::chrono::system_clock::to_time_t(now);

    // Convert to tm structure (local time)
    std::tm tm = *std::localtime(&time);

    // Format to string
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string generateFileName(const int hres, const int vres, const bool blur, const int NPR, const bool secRays, const float lensRadius, const float lensDepth) {
    return getCurrentDateTime() + " - " + stringify(hres) + "x" + stringify(vres) + " - blur(" + stringify(blur) +
           ") - " + "NPR(" + stringify(NPR) + ") - " + "secRays(" + stringify(secRays) + ")" + " - lensRadius(" +
           stringify((int) lensRadius) + ") - lensDepth(" + stringify((int) lensDepth) + ").ppm";
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string item;

    while (std::getline(ss, item, delimiter)) {
        tokens.push_back(item);
    }
    return tokens;
}