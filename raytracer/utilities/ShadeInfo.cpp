#include <iostream>
#include "ShadeInfo.hpp"

ShadeInfo::ShadeInfo(const World &wr){
    w = &wr;
    hit = false;
    depth = 0;
    t = 0;
}

