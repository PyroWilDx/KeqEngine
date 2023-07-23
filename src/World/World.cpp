//
// Created by pyrowildx on 25/06/2023.
//

#include "World/World.hpp"
#include "Utils/Events.hpp"

World::World(int screenW, int screenH,
             int backgroundTotalW, int backgroundTotalH,
             const char *backgroundImgPath) {
    background = new Background(screenW, screenH,
                                backgroundTotalW, backgroundTotalH,
                                backgroundImgPath);
    activeButton = nullptr;

    pixels = (Pixel **) new Pixel[backgroundTotalW];
    for (int i = 0; i < background->getTotalW(); i++) {
        pixels[i] = new Pixel[backgroundTotalH];
        for (int j = 0; j < backgroundTotalH; j++) {
            pixels[i][j] = {-1, -1};
        }
    }
}

World::~World() {
    for (Block *block: blockVector) {
        delete block;
    }

    for (std::pair<const int, Button *> it: buttonHashMap) {
        delete it.second;
    }

    for (int i = 0; i < background->getTotalW(); i++) {
        delete[] pixels[i];
    }
    delete[] pixels;

    delete background;
}

void World::updatePixels(int x1, int y1, int x2, int y2, WorldEntity *worldEntity) {
    for (int i = x1; i < x2; i++) {
        for (int j = y1; j < y2; j++) {
            pixels[i][j] = {worldEntity->getWorldEntityType(), worldEntity->getWorldCode()};
        }
    }
}

void World::addWorldEntity(WorldEntity *worldEntity) {
    double worldEntityW, worldEntityH;
    worldEntity->getRealSize(&worldEntityW, &worldEntityH);
    int minX = std::max(worldEntity->getX(), 0);
    int maxX = (int) std::min(worldEntity->getX() + worldEntityW, (double) background->getTotalW());
    int minY = std::max(worldEntity->getY(), 0);
    int maxY = (int) std::min(worldEntity->getY() + worldEntityH, (double) background->getTotalH());
    updatePixels(minX, minY, maxX, maxY, worldEntity);
}

void World::addBlock(Block *block) {
    blockVector.push_back(block);
    addWorldEntity(block);
}

void World::addBlock(int blockCode, double x, double y, int renderW, int renderH) {
    auto *block = new Block(blockCode, x, y, renderW, renderH);
    addBlock(block);
}

void World::addButton(Button *button) {
    buttonHashMap[button->getWorldCode()] = button;
    addWorldEntity(button);
}

Pixel World::getPixel(double x, double y) {
    if (x < 0 || x >= background->getTotalW() ||
        y < 0 || y >= background->getTotalH()) {
        return {WORLD_BLOCK, BLOCK_WALL_INVISIBLE};
    }
    return pixels[roundToInt(x)][roundToInt(y)];
}

bool World::isPixelBlock(double x, double y) {
    return (getPixel(x, y).worldType == WORLD_BLOCK);
}

bool World::isPixelButton(double x, double y) {
    return (getPixel(x, y).worldType == WORLD_BUTTON);
}

bool World::isPixelSurface(double x, double y) {
    return (isPixelBlock(x, y));
}

bool World::isPixelCode(double x, double y, int worldCode) {
    return (getPixel(x, y).worldCode == worldCode);
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
    while (pixels[i][j].worldCode != -1) {
        i += addX;
        j += addY;
    }
    if (addX == 0) return (j - addY);
    else return (i - addX);
}

void World::clickPixel(double x, double y, Uint32 eventType) {
    if (eventType == SDL_MOUSEBUTTONDOWN) {
        if (isPixelButton(x, y)) {
            int worldCode = getPixel(x, y).worldCode;
            auto buttonIterator = buttonHashMap.equal_range(worldCode);
            for (auto it = buttonIterator.first; it != buttonIterator.second; it++) {
                Button *button = it->second;
                if (button->getWorldCode() == worldCode) {
                    button->onClick((int) x, (int) y);
                    activeButton = button;
                    return;
                }
            }
        }
    }

    if (eventType == SDL_MOUSEBUTTONUP) {
        if (activeButton != nullptr) {
            bool isMouseOnButton = isPixelCode(x, y, activeButton->getWorldCode());
            activeButton->onClickRelease((int) x, (int) y, isMouseOnButton);
            activeButton = nullptr;
        }
    }
}

void World::renderSelf() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();
    gWindow->renderEntity(background);
    for (Block *block: blockVector) {
        gWindow->renderEntity(block);
    }
    for (std::pair<const int, Button *> it: buttonHashMap) {
        gWindow->renderEntity(it.second);
    }
}
