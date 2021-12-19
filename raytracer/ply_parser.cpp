#include <iostream>
#include <ostream>
#include <string>
#include "happly-master/happly.h"
#include "utilities/Point3D.hpp"
using namespace std;

int main(){

    happly::PLYData plyIn("dragon_vrip.ply");
    std::vector<array<double, 3>> vertices = plyIn.getVertexPositions();
    std::vector<vector<size_t>> faces = plyIn.getFaceIndices<size_t>();
    std::vector<Point3D> points;

    for (const auto& i : vertices) 
        {
            Point3D p(i[0], i[1], i[2]);
            points.push_back(p);
        }
    for (int i=0; i<points.size(); ++i)
    std::cout <<points[i].to_string() << '\n';
}




