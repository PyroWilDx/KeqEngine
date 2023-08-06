//
// Created by pyrowildx on 09/07/2023.
//

#include "BaseEntity/WorldEntity.hpp"

int WorldEntity::entityCount = 0;

WorldEntity::WorldEntity(double x, double y, int renderW, int renderH, WorldEntityType worldEntityType)
        : Entity(x, y) {
    this->worldEntityType = worldEntityType;
    this->worldCode = entityCount;
    entityCount++;
    this->renderW = renderW;
    this->renderH = renderH;
}

void WorldEntity::getRealSize(double *pW, double *pH) {
    if (pW != nullptr) *pW = renderW;
    if (pH != nullptr) *pH = renderH;
}

void WorldEntity::addImage(const char *imgPath, int frameW, int frameH) {
    setTexture(WindowRenderer::getInstance()->loadTexture(imgPath),
               frameW, frameH);
}

bool WorldEntity::isPixelInSelfRect(double pixelX, double pixelY) {
    return (pixelX >= x &&
            pixelX <= x + renderW &&
            pixelY >= y &&
            pixelY <= y + renderH);
}
