//
// Created by pyrow on 25/06/2023.
//

#include "World.hpp"

World::World(int screenW, int screenH,
             int backgroundTotalW, int backgroundTotalH,
             const char *backgroundImgPath)
        : background(new Background(
        screenW, screenH,
        backgroundTotalW, backgroundTotalH,
        WindowRenderer::getInstance()->loadTexture(backgroundImgPath))),
          pixels() {

    pixels = (int **) new int[backgroundTotalW];
    for (int i = 0; i < background->getTotalW(); i++) {
        pixels[i] = new int[backgroundTotalH];
        for (int j = 0; j < backgroundTotalH; j++) {
            pixels[i][j] = BLOCK_NULL;
        }
    }
}

World::~World() {
    for (Block *block: blocks) {
        delete block;
    }
}

void World::addBlock(Block *block) {
    blocks.push_back(block);
    int minX = max(block->getX(), 0);
    int maxX = min(block->getX() + block->getRenderW(), background->getTotalW());
    int minY = max(block->getY(), 0);
    int maxY = min(block->getY() + block->getRenderH(), background->getTotalH());

    for (int i = minX; i < maxX; i++) {
        for (int j = minY; j < maxY; j++) {
            pixels[i][j] = block->getCode();
        }
    }
}

void World::addBlock(int blockCode, double x, double y, int renderW, int renderH) {
    auto *block = new Block(blockCode, x, y, renderW, renderH);
    addBlock(block);
}

int World::getPixel(double x, double y) {
    if (x < 0 || x >= background->getTotalW() ||
        y < 0 || y >= background->getTotalH()) {
        return BLOCK_WALL_INVISIBLE;
    }
    return pixels[(int) x][(int) y];
}

double World::getNearestWallFrom(double x, double y, int direction) {
    int addX, addY;
    if (direction == KEY_Z) {
        addX = 0;
        addY = -1;
    } else if (direction == KEY_Q) {
        addX = -1;
        addY = 0;
    } else if (direction == KEY_S) {
        addX = 0;
        addY = 1;
    } else if (direction == KEY_D) {
        addX = 1;
        addY = 0;
    }

    if (x < 0) x = 0;
    if (x >= background->getTotalW()) x = background->getTotalW() - 1;
    if (y < 0) y = 0;
    if (y > background->getTotalH()) y = background->getTotalH() - 1;
    int i = (int) x;
    int j = (int) y;
    while (pixels[i][j] != BLOCK_NULL) {
        i += addX;
        j += addY;
    }
    if (addX == 0) return (j - addY);
    else return (i - addX);
}

void World::renderSelf() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();
    gWindow->renderEntity(background);
    for (Block *block: blocks) {
        gWindow->renderEntity(block);
    }
}
