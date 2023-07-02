//
// Created by pyrow on 25/06/2023.
//

#include "Block.hpp"
#include "WindowRenderer.hpp"
#include "Global.hpp"

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
        *pW = frame.w;
    if (pH != nullptr)
        *pH = frame.h;
}

//SDL_Rect Block::getRenderRect() {
//    double xCoeff = (double) Global::windowWidth / SCREEN_BASE_WIDTH;
//    double yCoeff = (double) Global::windowHeight / SCREEN_BASE_HEIGHT;
//    SDL_Rect backgroundFrame = Global::currentWorld->getBackground()->getFrame();
//    dst.x -= backgroundFrame.x;
//    dst.y -= backgroundFrame.y;
//    return {roundToInt(getX() * xCoeff),
//            roundToInt(getY() * yCoeff),
//            roundToInt(frame.w * xCoeff),
//            roundToInt(frame.h * yCoeff)};
//}
