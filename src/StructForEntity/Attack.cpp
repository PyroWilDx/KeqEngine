//
// Created by pyrowildx on 27/07/2023.
//

#ifdef _WIN32

#include <SDL2_gfx/SDL2_gfxPrimitives.h>

#else

#include <SDL2/SDL2_gfxPrimitives.h>

#endif

#include "StructForEntity/Attack.hpp"
#include "Utils/Global.hpp"
#include "BaseEntity/Entity.hpp"
#include "BaseEntity/LivingEntity.hpp"
#include "Utils/Utils.hpp"
#include "World/World.hpp"
#include "World/Background.hpp"
#include "Entity/Particle.hpp"
#include "Utils/Random.hpp"

#define BIG_PARTICLE_FRAME_LENGTH 40
#define SMALL_PARTICLE_FRAME_LENGTH BIG_PARTICLE_FRAME_LENGTH

void calcPoint(double *pX, double *pY,
               Entity *followEntity, double xyArray[][2], int i) {
    double fECenterX, fECenterY;
    followEntity->getSelfCenter(&fECenterX, &fECenterY);

    double xPoint = xyArray[i][0];
    if (!followEntity->isFacingEast()) xPoint = -xPoint;
    xPoint += fECenterX;
    double yPoint = xyArray[i][1] + fECenterY;

    double angle = followEntity->getRotation();
    angle = degreeToRad(angle);
    if (angle != 0) {
        rotatePoint(xPoint, yPoint, pX, pY,
                    fECenterX, fECenterY, angle, !followEntity->isFacingEast());

    } else {
        *pX = xPoint;
        *pY = yPoint;
    }
}

Attack::Attack(LivingEntity *atkIssuer_, Entity *followEntity,
               double xyArray[][2], int arrayLength,
               int damage, double kbXVelocity, double kbYVelocity) {
    this->atkIssuer = atkIssuer_;
    this->followEntity = followEntity;

    if (followEntity == nullptr) followEntity = atkIssuer_;

    this->followEntityLastX = followEntity->getX();
    this->followEntityLastY = followEntity->getY();

    double xPoint, yPoint;
    for (int i = 0; i < arrayLength; i++) {
        calcPoint(&xPoint, &yPoint,
                  followEntity, xyArray, i);
        bst_geo::append(atkPolygon, BoostPoint(xPoint, yPoint));
    }
    calcPoint(&xPoint, &yPoint,
              followEntity, xyArray, 0); // Close Polygon
    bst_geo::append(atkPolygon, BoostPoint(xPoint, yPoint));

    this->damage = damage;
    if (!followEntity->isFacingEast()) kbXVelocity = -kbXVelocity;
    this->kbXVelocity = kbXVelocity;
    this->kbYVelocity = kbYVelocity;
    this->atkTimeAcc = 0;
    this->atkDuration = 0;
    this->onHit = nullptr;
    this->onHitParams = nullptr;
    this->shouldRemove = nullptr;
    this->shouldRemoveParams = nullptr;
    this->bigParticle = nullptr;
    this->smallParticle = nullptr;
    this->uniqueEntityHit = false;
}

Attack::Attack(LivingEntity *atkIssuer_, double xyArray[][2], int arrayLength,
               int damage, double kbXVelocity, double kbYVelocity)
        : Attack(atkIssuer_, atkIssuer_,
                 xyArray, arrayLength,
                 damage, kbXVelocity, kbYVelocity) {

}

Attack::~Attack() {
    delete bigParticle;
    delete smallParticle;
}

void Attack::setClassicParticle(int n, bool electro) {
    const int nCode = 8;
    const int allCodes[nCode] = {
            PARTICLE_DMG_PHYSICAL_0,
            PARTICLE_DMG_PHYSICAL_1,
            PARTICLE_DMG_PHYSICAL_2,
            PARTICLE_DMG_PHYSICAL_MINI,
            PARTICLE_DMG_ELECTRO_0,
            PARTICLE_DMG_ELECTRO_1,
            PARTICLE_DMG_ELECTRO_2,
            PARTICLE_DMG_ELECTRO_MINI
    };
    int addI = (nCode / 2) * electro;
    bigParticle = new Particle(allCodes[n + addI],
                               BIG_PARTICLE_FRAME_LENGTH,
                               1., 1.);
    smallParticle = new Particle(allCodes[3 + addI],
                                 SMALL_PARTICLE_FRAME_LENGTH,
                                 1., 1.);
}

BoostPolygon Attack::getPolygonFromEntity(Entity *dstEntity) {
    SDL_Rect hitBox = dstEntity->getHitBox();
    double x1 = dstEntity->getX() + (double) hitBox.x;
    double x2 = x1 + (double) hitBox.w;
    double y1 = dstEntity->getY() + (double) hitBox.y;
    double y2 = y1 + (double) hitBox.h;
    BoostPolygon resPolygon = BoostPolygon();
    bst_geo::append(resPolygon, BoostPoint(x1, y1));
    bst_geo::append(resPolygon, BoostPoint(x2, y1));
    bst_geo::append(resPolygon, BoostPoint(x2, y2));
    bst_geo::append(resPolygon, BoostPoint(x1, y2));
    bst_geo::append(resPolygon, BoostPoint(x1, y1));
    return resPolygon;
}

bool Attack::isHittingEntity(LivingEntity *dstEntity) {
    BoostPolygon dstHitBoxPoly = getPolygonFromEntity(dstEntity);
    bool res = bst_geo::intersects(atkPolygon, dstHitBoxPoly);
    return res;
}

void Attack::checkEntityHit(LivingEntity *dstEntity) {
    if (uniqueEntityHit && !hitEntityVector.empty()) {
        shouldRemove = [](Attack *atk, void *fParams) {
            return true;
        };
        return;
    }

    if (isHittingEntity(dstEntity)) {
        if (std::find(hitEntityVector.begin(), hitEntityVector.end(),
                      dstEntity) == hitEntityVector.end()) {
            if (onHit != nullptr) onHit(this, dstEntity, onHitParams);
            bool isDamaged = dstEntity->damageSelf(damage,
                                                   kbXVelocity, kbYVelocity);
            if (isDamaged) {
                hitEntityVector.push_back(dstEntity);

                if (bigParticle != nullptr) {
                    bigParticle->moveToEntityCenter(dstEntity);
                    Particle *bigParticleClone = bigParticle->cloneSelf();
                    Particle::pushFast(bigParticleClone);
                }
                if (smallParticle != nullptr) {
                    double kbAngle = getAngleOrigin(kbXVelocity, kbYVelocity);
                    double maxGap = M_PI / 4.;
                    double minAngle = kbAngle - maxGap;
                    double maxAngle = kbAngle + maxGap;

                    smallParticle->moveToEntityCenter(dstEntity);
                    smallParticle->setRotationMovingRot(
                            Random::getRandomReal(minAngle, maxAngle));
                    const double smallParticleVelocity = 0.4;
                    smallParticle->setXYVelocity(smallParticleVelocity,
                                                 -smallParticleVelocity);
                    smallParticle->setOnRender([](Particle *particle) {
                        particle->moveXNoCheck();
                        particle->moveYNoCheck();
                    });
                    const int nSmallParticleCount = (int) (ATK_SMALL_PARTICLE_COUNT +
                                                           4. * getAddAbs(kbXVelocity, kbYVelocity));
                    for (int i = 0; i < nSmallParticleCount; i++) {
                        Particle *smallParticleClone = smallParticle->cloneSelf();

                        const double addWHMGap = 0.2;
                        double rdAddWHM = Random::getRandomReal(
                                -addWHMGap, addWHMGap);
                        smallParticleClone->addRenderWHMultiplier(
                                rdAddWHM, rdAddWHM);

                        const double addMoveGap = 10.;
                        smallParticleClone->moveAdd(
                                Random::getRandomReal(-addMoveGap, addMoveGap),
                                Random::getRandomReal(-addMoveGap, addMoveGap)
                        );

                        const double addVelocityGap = 0.1;
                        double rdAddVelocity = Random::getRandomReal(
                                -addVelocityGap, addVelocityGap);
                        smallParticleClone->addXYVelocity(
                                rdAddVelocity, rdAddVelocity);

                        double rdRotationRad = Random::getRandomReal(
                                minAngle, maxAngle);
                        smallParticleClone->setRotationMovingRot(
                                radToDegree(rdRotationRad));

                        const int addFrameTimeGap = 20;
                        int baseFrameTime = SMALL_PARTICLE_FRAME_LENGTH;
                        smallParticleClone->setSpriteFrameLengthFromTo(
                                Random::getRandomInt(
                                        baseFrameTime - addFrameTimeGap,
                                        baseFrameTime + addFrameTimeGap / 2),
                                0, -1);

                        Particle::pushFast(smallParticleClone);
                    }
                }
            }
        }
    }
}

void Attack::onGameFrame() {
    if (followEntity != nullptr) {
        double atkIssuerX = followEntity->getX();
        double atkIssuerY = followEntity->getY();
        double xDiff = atkIssuerX - followEntityLastX;
        double yDiff = atkIssuerY - followEntityLastY;
        for (BoostPoint &point: atkPolygon.outer()) {
            point.x(point.x() + xDiff);
            point.y(point.y() + yDiff);
        }
        followEntityLastX = atkIssuerX;
        followEntityLastY = atkIssuerY;
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
    SDL_Log("Attack has no way to be removed !");
    return false;
}
