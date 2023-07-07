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
            imgFrame.w = 0;
            imgFrame.h = 0;
            imgTexture = nullptr;
            return;
        case BLOCK_DIRT:
            imgFrame.w = 32;
            imgFrame.h = 32;
            imgPath = "res/gfx/block/Dirt.png";
            break;
        default:
            break;
    }
    imgTexture = WindowRenderer::getInstance()->loadTexture(imgPath);
}

void Block::getRealSize(double *pW, double *pH) {
    if (pW != nullptr)
        *pW = imgFrame.w;
    if (pH != nullptr)
        *pH = imgFrame.h;
}

void Block::renderSelf(SDL_Renderer *gRenderer) {
    SDL_Rect src = this->getFrame();

    if (src.x < 0 || src.y < 0) return;
    if (src.w <= 0 || src.h <= 0) return;

    SDL_Rect dst = this->getRenderRect();

    SDL_RendererFlip renderFlip = SDL_FLIP_NONE;
    double renderRotation = this->getRotation();
    if (!this->isFacingEast()) {
        renderFlip = SDL_FLIP_HORIZONTAL;
        renderRotation = -renderRotation;
    }

    int baseX = dst.x;
    int baseY = dst.y;
    double xCoeff = ((double) this->getRenderW() / (double) this->getFrame().w);
    double yCoeff = ((double) this->getRenderH() / (double) this->getFrame().h);
    for (int i = 0; i < (int) xCoeff; i++) {
        for (int j = 0; j < (int) yCoeff; j++) {
            SDL_RenderCopyEx(gRenderer, this->getTexture(),
                             &src, &dst,
                             renderRotation, nullptr, renderFlip);
            dst.y += dst.h;
        }
        dst.y = baseY;
        dst.x += dst.w;
    }

    double xLeftover = xCoeff - (int) xCoeff;
    double yLeftover = yCoeff - (int) yCoeff;
    double epsilon = 0.0001;

    if (xLeftover <= epsilon && yLeftover <= epsilon) return;

    int lastX = baseX + dst.w * (int) xCoeff;
    int lastY = baseY + dst.h * (int) yCoeff;
    int srcBaseW = src.w;
    int srcBaseH = src.h;
    int dstBaseW = dst.w;
    int dstBaseH = dst.h;
    double srcLeftoverW = (double) srcBaseW * xLeftover;
    double srcLeftoverH = (double) srcBaseH * yLeftover;
    double wLeftoverCoeff = (double) dstBaseW / (double) srcBaseW;
    double hLeftoverCoeff = (double) dstBaseH / (double) srcBaseH;
    if (xLeftover > epsilon) {
        src.w = roundToInt(srcLeftoverW);
        src.h = srcBaseH;
        dst.x = lastX;
        dst.y = baseY;
        dst.w = roundToInt(srcLeftoverW * wLeftoverCoeff);
        dst.h = dstBaseH;
        for (int j = 0; j < (int) yCoeff; j++) {
            SDL_RenderCopyEx(gRenderer, this->getTexture(),
                             &src, &dst,
                             renderRotation, nullptr, renderFlip);
            dst.y += dst.h;
        }
    }
    if (yLeftover > epsilon) {
        src.w = srcBaseW;
        src.h = roundToInt(srcLeftoverH);
        dst.x = baseX;
        dst.y = lastY;
        dst.w = dstBaseW;
        dst.h = roundToInt(srcLeftoverH * hLeftoverCoeff);
        for (int i = 0; i < (int) xCoeff; i++) {
            SDL_RenderCopyEx(gRenderer, this->getTexture(),
                             &src, &dst,
                             renderRotation, nullptr, renderFlip);
            dst.x += dst.w;
        }
    }
    if (xLeftover > epsilon && yLeftover > epsilon) {
        src.w = roundToInt(srcLeftoverW);
        src.h = roundToInt(srcLeftoverH);
        dst.x = lastX;
        dst.y = lastY;
        dst.w = roundToInt(srcLeftoverW * wLeftoverCoeff);
        dst.h = roundToInt(srcLeftoverH * hLeftoverCoeff);
        SDL_RenderCopyEx(gRenderer, this->getTexture(),
                         &src, &dst,
                         renderRotation, nullptr, renderFlip);
    }
}
