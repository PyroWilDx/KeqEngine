//
// Created by pyrow on 27/07/2023.
//

#ifdef _WIN32

#include <SDL2_gfx/SDL2_gfxPrimitives.h>

#else

#include <SDL2/SDL2_gfxPrimitives.h>

#endif

#include "StructForEntity/Attack.hpp"
#include "Utils/Global.hpp"

Attack::Attack(LivingEntity *atkIssuer_, double xyArray[][2], int arrayLength,
               int damage, double kbXVelocity, double kbYVelocity) {
    this->atkIssuer = atkIssuer_;
    this->atkIssuerDependant = true;
    this->atkIssuerLastX = atkIssuer->getX();
    this->atkIssuerLastY = atkIssuer->getY();
    for (int i = 0; i < arrayLength; i++) {
        double xPoint = atkIssuer->getX() + atkIssuer->getBaseHitBoxX() + atkIssuer->getHalfBaseHitBoxW();
        if (atkIssuer->isFacingEast()) xPoint += xyArray[i][0];
        else xPoint -= xyArray[i][0];
        double yPoint = atkIssuer->getY() + atkIssuer->getBaseHitBoxY() +
                        atkIssuer->getHalfBaseHitBoxH() + xyArray[i][1];
        bst_geo::append(atkPolygon, BoostPoint(xPoint, yPoint));
    }
    this->damage = damage;
    if (!atkIssuer->isFacingEast()) kbXVelocity = -kbXVelocity;
    this->kbXVelocity = kbXVelocity;
    this->kbYVelocity = kbYVelocity;
    this->atkTimeAcc = 0;
    this->atkDuration = 0;
    this->shouldRemove = nullptr;
    this->shouldRemoveParams = nullptr;
}

BoostPolygon *Attack::getPolygonFromEntity(Entity *dstEntity) {
    SDL_Rect hitBox = dstEntity->getHitBox();
    double x1 = dstEntity->getX() + (double) hitBox.x;
    double x2 = x1 + (double) hitBox.w;
    double y1 = dstEntity->getY() + (double) hitBox.y;
    double y2 = y1 + (double) hitBox.h;
    auto *resPolygon = new BoostPolygon;
    bst_geo::append(*resPolygon, BoostPoint(x1, y1));
    bst_geo::append(*resPolygon, BoostPoint(x2, y1));
    bst_geo::append(*resPolygon, BoostPoint(x2, y2));
    bst_geo::append(*resPolygon, BoostPoint(x1, y2));
    return resPolygon;
}

bool Attack::isHittingEntity(LivingEntity *dstEntity) {
    BoostPolygon *dstHitBoxPoly = getPolygonFromEntity(dstEntity);
    bool res = bst_geo::intersects(atkPolygon, *dstHitBoxPoly);
    delete dstHitBoxPoly;
    return res;
}

void Attack::checkEntityHit(LivingEntity *dstEntity) {
    if (isHittingEntity(dstEntity)) {
        dstEntity->damageSelf(damage, kbXVelocity, kbYVelocity);
    }
}

void Attack::onGameFrame() {
    if (atkIssuerDependant) {
        double atkIssuerX = atkIssuer->getX();
        double atkIssuerY = atkIssuer->getY();
        double xDiff = atkIssuerX - atkIssuerLastX;
        double yDiff = atkIssuerY - atkIssuerLastY;
        for (BoostPoint &point: atkPolygon.outer()) {
            point.x(point.x() + xDiff);
            point.y(point.y() + yDiff);
        }
        atkIssuerLastX = atkIssuerX;
        atkIssuerLastY = atkIssuerY;
    }
}

void Attack::renderSelf(SDL_Renderer *gRenderer) {
    double xCoeff, yCoeff;
    getScreenXYCoeff(&xCoeff, &yCoeff);
    const size_t length = bst_geo::num_points(atkPolygon);
    Sint16 xArray[length], yArray[length];
    int i = 0;
    for (const BoostPoint &point: atkPolygon.outer()) {
        double xDouble = point.x() - (double) Global::currentWorld->getBackground()->getFrame().x;
        double yDouble = point.y() - (double) Global::currentWorld->getBackground()->getFrame().y;
        xDouble *= xCoeff;
        yDouble *= yCoeff;
        int xInt = (int) xDouble;
        int yInt = (int) yDouble;
        shiftXYFromScreenPosition(&xInt, &yInt);
        xArray[i] = (Sint16) xInt;
        yArray[i] = (Sint16) yInt;
        i++;
    }
    filledPolygonRGBA(gRenderer,
                      xArray, yArray,
                      (int) length,
                      COLOR_GREEN, 180);
    SDL_SetRenderDrawColor(gRenderer, COLOR_BLACK_FULL);
}

bool Attack::shouldSelfRemove() {
    if (shouldRemove != nullptr) {
        return shouldRemove(this, shouldRemoveParams);
    }
    if (atkDuration != 0) {
        atkTimeAcc += Global::dt;
        return (atkTimeAcc > atkDuration);
    }
    SDL_Log("Attack has no way to be removed !\n");
    return false;
}
