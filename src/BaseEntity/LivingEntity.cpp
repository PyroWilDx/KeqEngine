//
// Created by pyrow on 24/07/2023.
//

#include <algorithm>
#include "BaseEntity/LivingEntity.hpp"
#include "Utils/Global.hpp"

LivingEntity::LivingEntity(double gravityWeight, int baseHp,
                           int spriteArrayLength, int hurtSpriteCode,
                           int stateChangerEndSpriteCode)
        : AnimatedEntity(spriteArrayLength) {
    this->gravityWeight = gravityWeight;
    this->hp = baseHp;
    this->hurtSpriteCode = hurtSpriteCode;
    this->hurtKbVX = 0;
    this->hurtKbVY = 0;
    this->hurtStartTime = 0;
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

void LivingEntity::setDmgFacingEast(double kbVX) {
    if (kbVX != 0) facingEast = (kbVX < 0);
}

void LivingEntity::damageSelf(int damage, double kbVX, double kbVY) {
    hp -= damage;
    hurtKbVY = kbVY;
    yVelocity = kbVY;
    setDmgFacingEast(kbVX);
    if (!isFacingEast()) kbVX = -kbVX;
    hurtKbVX = kbVX;
    xVelocity = kbVX;
    hurtStartTime = Global::currentTime;
    setSpriteAnimated(true, hurtSpriteCode);
}

void LivingEntity::hurt() {
    auto minF =
            [](double a, double b) { return std::min(a, b); };
    auto maxF =
            [](double a, double b) { return std::max(a, b); };
    double (*fX)(double, double) = (xVelocity < 0) ? minF : maxF;
    double (*fY)(double, double) = (yVelocity < 0) ? minF : maxF;
    double addX = (xVelocity < 0) ? 0.001 : -0.001;
    double addY = (yVelocity < 0) ? 0.001 : -0.001;

    xVelocity = fX(xVelocity + addX * Global::dt, 0.0);
    yVelocity = fY(yVelocity + addY * Global::dt, 0.0);

    if (isHittingWallHorizontally()) {
        xVelocity /= 2;
        xVelocity = -xVelocity;
    }

    if (!isInAir()) {
        xVelocity /= 1.06;
    }

    if (isHittingCeiling()) {
        yVelocity /= 2;
        yVelocity = -yVelocity;
    }

    if (xVelocity == 0 && yVelocity == 0) {
        setSpriteAnimated(false, hurtSpriteCode);
    }
}

void LivingEntity::updateAction() {
    int currSpriteCode = getCurrentSpriteCode();
//    int startIndex = (currSpriteCode > stateChangerEndSpriteCode) ? stateChangerEndSpriteCode + 1 : 0;
    int startIndex = 0;

    for (int i = startIndex; i < currSpriteCode; i++) {
        setSpriteAnimated(false, i);
    }
}
