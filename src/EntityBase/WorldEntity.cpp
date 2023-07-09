//
// Created by pyrow on 09/07/2023.
//

#include "EntityBase/WorldEntity.hpp"

int WorldEntity::entityCount = 0;

WorldEntity::WorldEntity(double x, double y, int renderW, int renderH, WorldEntityType worldEntityType)
        : Entity(x, y) {
    this->worldEntityType = worldEntityType;
    this->worldCode = entityCount;
    entityCount++;
    this->renderW = renderW;
    this->renderH = renderH;
}

void WorldEntity::addImage(const char *imgPath, int frameW, int frameH) {
    imgTexture = WindowRenderer::getInstance()->loadTexture(imgPath);
    if (frameW != 0 && frameH != 0) {
        imgFrame.w = frameW;
        imgFrame.h = frameH;
    } else {
        SDL_QueryTexture(imgTexture, nullptr, nullptr, &imgFrame.w, &imgFrame.h);
    }
}

void WorldEntity::getRealSize(double *pW, double *pH) {
    if (pW != nullptr) *pW = renderW;
    if (pH != nullptr) *pH = renderH;
}
