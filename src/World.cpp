//
// Created by pyrow on 25/06/2023.
//

#include "World.hpp"

World::World(int backgroundW, int backgroundH, int backgroundTotalW, const char *backgroundImgPath) {
    WindowRenderer *gWindow = WindowRenderer::getInstance();
    background = new Background(backgroundW, backgroundH, backgroundTotalW,
                                gWindow->loadTexture(backgroundImgPath));
}