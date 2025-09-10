//
// Created by Owais Bin Asad on 10/09/2025.
//

#include "FileReader.h"

#include <fstream>
#include <iostream>

std::vector<std::string> getLinesFromFile(const std::string &filename) {
    std::fstream file;
    std::vector<std::string> lines;

    file.open(filename, std::ios::in);

    if (file.is_open()) {
        std::string line;
        std::cout << "File " + filename + " opened successfully." << std::endl;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
    }

    file.close();
    std::cout << "File " + filename + " closed." << std::endl;
    std::cout << "Read " << lines.size() << " lines." << std::endl;
    return lines;
}
