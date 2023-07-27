//
// Created by pyrow on 27/07/2023.
//

#include "Attack.hpp"

Attack::Attack(LivingEntity *atkIssuer_, double xyArray[][2], int arrayLength,
               int damage, double kbXVelocity, double kbYVelocity) {
    this->atkIssuer = atkIssuer_;
    for (int i = 0; i < arrayLength; i++) {
        // TODO FAUDRA SUREMENT MULTIPLIER PAR RENDERWHM LES HITBOX WH ? A DEBUG PLUS TARD
        bst_geo::append(atkPolygon,
                        BoostPoint(xyArray[i][0],
                                   xyArray[i][1]));
        SDL_Log("%f %f\n", xyArray[i][0],
                xyArray[i][1]);
    }
    this->damage = damage;
    this->kbXVelocity = kbXVelocity;
    this->kbYVelocity = kbYVelocity;
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
