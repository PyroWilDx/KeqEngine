//
// Created by pyrowildx on 09/07/2023.
//

#include "AbstractEntity/WorldEntity.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Global.hpp"
#include "World/World.hpp"

int WorldEntity::entityCount = 0;

WorldEntity::WorldEntity(double x, double y, int renderW, int renderH, WorldEntityType worldEntityType)
        : Entity(x, y) {
    this->worldEntityType = worldEntityType;
    this->worldCode = entityCount;
    entityCount++;
    this->renderW = renderW;
    this->renderH = renderH;
}

void WorldEntity::setHitBoxAuto() {
    hitBox = {0, 0, renderW, renderH};
}

void WorldEntity::resizeToRenderSize() {
    imgFrame.w = getRenderW();
    imgFrame.h = getRenderH();
}

void WorldEntity::moveTo(double x_, double y_) {
    SDL_Rect lastRect = {getX(), getY(), renderW, renderH};

    Entity::moveTo(x_, y_);

    Global::gWorld->refreshPixelsOnMove(this, &lastRect);
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

bool WorldEntity::getCollisionArea(SDL_Rect *collRect, SDL_Rect *resultRect) {
    SDL_Rect rect1 = {getX(), getY(), renderW, renderH};
    return SDL_IntersectRect(&rect1, collRect, resultRect);
}

bool WorldEntity::getCollisionArea(WorldEntity *worldEntity, SDL_Rect *resultRect) {
    SDL_Rect rect1 = {getX(), getY(), renderW, renderH};
    SDL_Rect rect2 = {worldEntity->getX(), worldEntity->getY(),
                      worldEntity->renderW, worldEntity->renderH};
    return SDL_IntersectRect(&rect1, &rect2, resultRect);
}
