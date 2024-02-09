//
// Created by pyrowildx on 27/07/2023.
//

#ifdef _WIN32

#include <SDL2_gfx/SDL2_gfxPrimitives.h>

#else

#include <SDL2/SDL2_gfxPrimitives.h>

#endif

#include <deque>
#include "EntityInfo/Attack.hpp"
#include "Utils/Global.hpp"
#include "AbstractEntity/Entity.hpp"
#include "AbstractEntity/LivingEntity.hpp"
#include "Utils/Utils.hpp"
#include "World/World.hpp"
#include "World/Background.hpp"
#include "Entity/Particle.hpp"
#include "Utils/Random.hpp"
#include "Utils/Sound.hpp"

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
               int atkDamage, double kbXVelocity, double kbYVelocity) {
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
    bst_geo::correct(atkPolygon);

    this->atkDamage = atkDamage;
    if (!followEntity->isFacingEast()) kbXVelocity = -kbXVelocity;
    this->kbXVelocity = kbXVelocity;
    this->kbYVelocity = kbYVelocity;
    this->atkTimeAcc = 0;
    this->atkDuration = 0;
    this->onHit = nullptr;
    this->onHitParams = nullptr;
    this->shouldRemove = nullptr;
    this->shouldRemoveParams = nullptr;
    this->hitSoundPath.clear();
    this->bigParticle = nullptr;
    this->smallParticle = nullptr;
    this->uniqueEntityHit = false;
    this->atkCompanion = nullptr;
}

Attack::Attack(LivingEntity *atkIssuer_, double xyArray[][2], int arrayLength,
               int atkDamage, double kbXVelocity, double kbYVelocity)
        : Attack(atkIssuer_, atkIssuer_,
                 xyArray, arrayLength,
                 atkDamage, kbXVelocity, kbYVelocity) {

}

Attack::~Attack() {
    delete bigParticle;
    delete smallParticle;
}

void Attack::setHitSound(const char *fileName) {
    hitSoundPath = std::string("res/sfx/particle/") + fileName;
}

void Attack::setHitSound(std::string &fileName) {
    setHitSound(fileName.c_str());
}

void Attack::setKQHitSoundRandom(int atkStrength) {
    std::string startName;
    int nSound = 0;
    if (atkStrength == 0) {
        startName = std::string("KQHitSwordHitWeak");
        nSound = 3;
    } else if (atkStrength == 1) {
        startName = std::string("KQHitSwordHitMedium");
        nSound = 2;
    } else if (atkStrength == 2) {
        startName = std::string("KQHitSwordStrong");
        nSound = 2;
    }

    int rdSound = Random::getRandomIntEndExcl(0, nSound);
    std::string finalName = startName + std::to_string(rdSound) + ".ogg";
    setHitSound(finalName);
}

void Attack::setClassicParticle(int n, bool electro) {
    myAssert(n >= 0 && n <= 2, "Index must be 0, 1 or 2");

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
    bst_geo::correct(resPolygon);
    return resPolygon;
}

BoostPoint Attack::isHittingEntity(LivingEntity *dstEntity) {
    BoostPolygon dstHitBoxPoly = getPolygonFromEntity(dstEntity);
    BoostPoint centerPoint;

    std::deque<BoostPolygon> qIntersection;
    bst_geo::intersection(atkPolygon, dstHitBoxPoly,
                          qIntersection);
    if (!qIntersection.empty()) {
        bst_geo::centroid(qIntersection[0], centerPoint);
    } else {
        centerPoint.x(-1.);
        centerPoint.y(-1.);
    }
    return centerPoint;
}

void Attack::checkEntityHit(LivingEntity *dstEntity) {
    if (uniqueEntityHit && !hitEntityVector.empty()) {
        shouldRemove = [](Attack *atk, void *fParams) {
            return true;
        };
        return;
    }

    if (haveHitEntity(dstEntity)) return;
    if (atkCompanion != nullptr && atkCompanion->haveHitEntity(dstEntity)) return;

    BoostPoint centerPoint = isHittingEntity(dstEntity);
    if (centerPoint.x() == -1.) return;

    const double maxPercentGap = 0.06;
    double lowerBoundPercent = 1. - maxPercentGap;
    double upperBoundPercent = 1. + maxPercentGap;
    double rdKbXV = kbXVelocity * Random::getRandomReal(lowerBoundPercent, upperBoundPercent);
    double rdKbYV = kbYVelocity * Random::getRandomReal(lowerBoundPercent, upperBoundPercent);
    bool isDamaged = dstEntity->damageSelf(atkDamage, rdKbXV, rdKbYV);

    if (!isDamaged) return;

    if (onHit != nullptr) onHit(this, dstEntity, onHitParams);
    hitEntityVector.push_back(dstEntity);

    if (!hitSoundPath.empty()) {
        Sound::playAudioChunk(hitSoundPath.c_str());
    }

    if (bigParticle != nullptr) {
        bigParticle->moveToCenter(centerPoint.x(), centerPoint.y());
        Particle *bigParticleClone = bigParticle->cloneSelf();
        Particle::pushFast(bigParticleClone);
    }
    if (smallParticle != nullptr) {
        double kbAngle = getAngleOrigin(kbXVelocity, kbYVelocity);
        double maxGap = M_PI / 4.;
        double minAngle = kbAngle - maxGap;
        double maxAngle = kbAngle + maxGap;

        smallParticle->moveToCenter(centerPoint.x(), centerPoint.y());
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

bool Attack::haveHitEntity(LivingEntity *checkEntity) {
    return std::find(hitEntityVector.begin(), hitEntityVector.end(),
                     checkEntity) != hitEntityVector.end();
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
        double xDouble = point.x() - (double) Global::gWorld->getBackground()->getFrame().x;
        double yDouble = point.y() - (double) Global::gWorld->getBackground()->getFrame().y;
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
