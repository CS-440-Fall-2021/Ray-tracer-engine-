#include <iostream>
#include <ostream>
#include <string>
#include "happly-master/happly.h"
#include "utilities/Point3D.hpp"
using namespace std;

int main(){

    happly::PLYData plyIn("mug.ply");
    std::cout << "Reading file...\n";
    std::vector<array<double, 3>> vertices = plyIn.getVertexPositions();
    std::vector<vector<size_t>> faces = plyIn.getFaceIndices<size_t>();
    std::cout << "File Read.\n";
    std::vector<Point3D> points;

    for (const auto& i : vertices) 
        {
            Point3D p(i[0], i[1], i[2]);
            points.push_back(p);
        }

    for (auto face : faces) {
        std::vector<Point3D> triangle;
        std::cout << "Triangle: ";
        for (int ind = 0; ind < 3; ind++)
        {
            // triangle.push_back(points[ind]);
            std::cout << points[face[ind]].to_string() + " ";
        }
        std::cout << "\n";
    }
}




