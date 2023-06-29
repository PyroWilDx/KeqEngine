//
// Created by pyrow on 25/06/2023.
//

#include "Block.hpp"
#include "WindowRenderer.hpp"

Block::Block(int blockCode, double x, double y, int renderW, int renderH)
        : Entity(x, y) {
    this->code = blockCode;
    this->renderW = renderW;
    this->renderH = renderH;

    setBlockInfo();
}

void Block::setBlockInfo() {
    const char *imgPath;
    switch (code) {
        case BLOCK_WALL_INVISIBLE:
            frame.w = 0;
            frame.h = 0;
            texture = nullptr;
            return;
        case BLOCK_DIRT:
            frame.w = 32;
            frame.h = 32;
            imgPath = "res/gfx/block/Dirt.png";
            break;
        default:
            break;
    }
    texture = WindowRenderer::getInstance()->loadTexture(imgPath);
}

void Block::getRealSize(double *pW, double *pH) {
    if (pW != nullptr)
        *pW = renderW;
    if (pH != nullptr)
        *pH = renderH;
}

SDL_Rect Block::getRenderRect() {
    return {getX(), getY(), frame.w, frame.h};
}
