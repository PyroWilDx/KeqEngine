//
// Created by pyrow on 27/07/2023.
//

#include "Attack.hpp"
#include "SDL2_gfx/SDL2_gfxPrimitives.h"
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
    return bst_geo::intersects(atkPolygon, *dstHitBoxPoly);
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
    const size_t length = bst_geo::num_points(atkPolygon);
    Sint16 xArray[length], yArray[length];
    int i = 0;
    for (const BoostPoint &point: atkPolygon.outer()) {
        xArray[i] = (Sint16) (point.x() - Global::currentWorld->getBackground()->getFrame().x);
        yArray[i] = (Sint16) (point.y() - Global::currentWorld->getBackground()->getFrame().y);
        i++;
    }
    filledPolygonRGBA(gRenderer,
                      xArray, yArray,
                      (int) length,
                      0, 255, 0, 180);
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
