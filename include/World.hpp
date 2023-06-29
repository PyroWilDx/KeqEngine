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
    World(int screenW, int screenH,
          int backgroundTotalW, int backgroundTotalH,
          const char *backgroundImgPath);

    ~World();

    void addBlock(Block *block);

    void addBlock(int blockCode, double x, double y, int renderW, int renderH);

    int getPixel(double x, double y);

    double getNearestWallFrom(double x, double y, int direction);

    void renderSelf();

    inline Background *getBackground() { return background; }

private:
    Background *background;
    vector<Block *> blocks;
    int **pixels;

};

#endif
