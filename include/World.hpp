//
// Created by pyrow on 25/06/2023.
//

#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "Background.hpp"
#include "Block.hpp"

using namespace std;

class World {

public:
    World(int backgroundW, int backgroundH, int backgroundTotalW, const char *backgroundImgPath);

    inline Background *getBackground() { return background; }

private:
    Background *background;
    vector<Block> blocks;
    int **pixels;

};

#endif
