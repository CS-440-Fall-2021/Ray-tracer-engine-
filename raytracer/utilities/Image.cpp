#include <iostream>
#include <fstream>
#include "Image.hpp"
#include "../world/ViewPlane.hpp"
#include "RGBColor.hpp"


Image::Image(int hres, int vres) {
    this->hres = hres;
    this->vres = vres;

    // Setting up a 2D array to store RGBColor objects
    this->colors = new RGBColor* [hres];
    for (int i=0 ; i < hres; ++i){
        colors[i] = new RGBColor [vres];
    }
}

Image::Image(const ViewPlane &vp) {
    this->hres = vp.hres;
    this->vres = vp.vres;

    // Setting up a 2D array to store RGBColor objects
    this->colors = new RGBColor* [hres];
    for (int i=0 ; i < hres; ++i){
        colors[i] = new RGBColor [vres];
    }
}

void Image::set_pixel(int x, int y, const RGBColor& color) {
    colors[x][y] = RGBColor();
    colors[x][y].r = color.r;
    colors[x][y].g = color.g;
    colors[x][y].b = color.b;
}

Image::~Image() {
    this->vres = 0;
    this->hres = 0;
    for (int i = 0; i < hres; i++)
    {
        free(colors[i]);
    }
    free(colors);
}

void Image::write_ppm(std::string path) const {
    std::fstream file;
    file.open(path, std::ios::out); // Open file in write mode.

    if(file.is_open()) {
        std::cout << "Writing to file...\n";
        file << "P3\n";
        file << std::to_string(hres) + " " + std::to_string(vres) + "\n";
        file << "255\n";
        
        for (int y = 0; y < vres; y++)
        {
            for (int x = 0; x < hres; x++)
            {
                file << colors[x][y].to_PPM_string();
            }
            
        }
        std::cout << "Write finsihed.\n";
        file.close();
        std::cout << "Filed closed.\n";
    } else {
        std::cout << "Couldn't open file!\n";
    }

}