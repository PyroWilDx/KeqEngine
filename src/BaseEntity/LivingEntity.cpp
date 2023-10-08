//
// Created by pyrowildx on 24/07/2023.
//

#include <algorithm>
#include "BaseEntity/LivingEntity.hpp"
#include "Utils/Global.hpp"
#include "Utils/Utils.hpp"

LivingEntity::LivingEntity(double gravityWeight, int baseHp,
                           int spriteArrayLength, int hurtSpriteCode,
                           int stateChangerEndSpriteCode)
        : AnimatedEntity(spriteArrayLength) {
    this->gravityWeight = gravityWeight;
    this->hp = baseHp;
    this->hurtSpriteCode = hurtSpriteCode;
    this->hurtKbXV = 0;
    this->hurtKbVY = 0;
    this->hitLagTime = 0;
    this->timeSinceHurt = 0;
    this->stateChangerEndSpriteCode = stateChangerEndSpriteCode;
    this->xShifts = new int[spriteArrayLength];
    this->yShifts = new int[spriteArrayLength];
    this->xRShifts = new int[spriteArrayLength];
    std::fill(xShifts, xShifts + spriteArrayLength, 0);
    std::fill(yShifts, yShifts + spriteArrayLength, 0);
    std::fill(xRShifts, xRShifts + spriteArrayLength, 0);
}

LivingEntity::~LivingEntity() {
    delete[] xShifts;
    delete[] yShifts;
    delete[] xRShifts;
}

void LivingEntity::setXYShift(int xShift, int yShift, int xRShift, int spriteCode) {
    xShifts[spriteCode] = xShift;
    yShifts[spriteCode] = yShift;
    xRShifts[spriteCode] = xRShift;
}

SDL_Rect LivingEntity::getRenderRect() {
    SDL_Rect dst = Entity::getRenderRect();

    double xCoeff, yCoeff;
    getScreenXYCoeff(&xCoeff, &yCoeff);
    xCoeff *= renderWMultiplier;
    yCoeff *= renderHMultiplier;

    if (facingEast) dst.x += (int) ((double) xShifts[getCurrentSpriteCode()] * xCoeff);
    else dst.x += (int) ((double) xRShifts[getCurrentSpriteCode()] * xCoeff);

    dst.y += (int) ((double) yShifts[getCurrentSpriteCode()] * yCoeff);

    return dst;
}

bool LivingEntity::onGameFrame() {
    bool doNext = Entity::onGameFrame();

    if (doNext) {
        if (isHurt()) {
            timeSinceHurt += Global::dt;

            if (timeSinceHurt < hitLagTime) return false;
        }
    }

    return doNext;
}

bool LivingEntity::isInvincible() {
    return false;
}

void LivingEntity::setDmgFacingEast(double kbXV) {
    if (kbXV != 0) facingEast = (kbXV < 0);
}

bool LivingEntity::damageSelf(int damage, double kbXV, double kbVY) {
    if (isInvincible()) return false;

    hp -= damage;
    hurtKbVY = kbVY;
    yVelocity = kbVY;
    setDmgFacingEast(kbXV);
    if (!isFacingEast()) kbXV = -kbXV;
    hurtKbXV = kbXV;
    xVelocity = kbXV;
    const double minKbAdded = 0.4;
    const double maxKbAdded = 2.;
    const double diffKbAdded = maxKbAdded - minKbAdded;
    double addLockTime = getAddAbs(kbXV, kbVY);
    addLockTime = std::min(addLockTime, maxKbAdded);
    addLockTime = std::max(addLockTime, minKbAdded);
    addLockTime -= minKbAdded;
    hitLagTime = (int) (ENTITY_MIN_HURT_LOCK_TIME +
                        addLockTime * (ENTITY_DIFF_HURT_LOCK_TIME / diffKbAdded));
    timeSinceHurt = 0;
    setSpriteAnimated(true, hurtSpriteCode);

    return true;
}

void LivingEntity::hurt() {
    double addX = 0.0012;
    xVelocity = (xVelocity < 0) ?
                std::min(xVelocity + addX * Global::dt, 0.) :
                std::max(xVelocity - addX * Global::dt, 0.);

    if (timeSinceHurt > 0) {
        if (isHittingWallHorizontally()) {
            xVelocity /= 2.;
            xVelocity = -xVelocity;
        }

        if (!isInAir()) {
            xVelocity /= 1.04;
        }
    }

    if (xVelocity == 0 && yVelocity == 0) {
        setSpriteAnimated(false, hurtSpriteCode);
    }
}

void LivingEntity::updateAction() {
    int currSpriteCode = getCurrentSpriteCode();

    for (int i = 0; i < currSpriteCode; i++) {
        setSpriteAnimated(false, i);
    }

    if (isHurt()) this->hurt();
}
