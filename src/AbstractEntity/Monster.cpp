//
// Created by pyrowildx on 24/07/2023.
//

#include "AbstractEntity/Monster.hpp"

Monster::Monster(double gravityWeight, int baseHp,
                 int spriteArrayLength, int hurtSpriteCode,
                 int stateChangerEndSpriteCode, int frameW)
        : LivingEntity(gravityWeight, baseHp,
                       spriteArrayLength, hurtSpriteCode,
                       stateChangerEndSpriteCode) {
    this->frameW = frameW;
    this->doAI = true;
}

void Monster::moveToEntityCenterFront(Entity *centerEntity, bool takeFaceEast) {
    moveToEntityCenter(centerEntity, takeFaceEast);
    if (centerEntity->isFacingEast()) x += (getRenderWMultiplier() * frameW) / 2.;
    else x -= (getRenderWMultiplier() * frameW) / 2.;
}

bool Monster::onGameFrame() {
    bool doNext = LivingEntity::onGameFrame();

    if (doNext) {
        fallGravity();
        if (!isDead) {
            if (doAI) AI();
            else setSpriteAnimated(true, 0); // Idle
            updateAction();
        } else {
            bool deletedSelf = animDeath();
            if (deletedSelf) return false;
            hurt();
        }
        moveX();
        moveY();
    }

    animateSprite();

    return doNext;
}

bool Monster::onDeath() {
    doAI = false;
    return false;
}

void Monster::updateAction() {
    LivingEntity::updateAction();

    bool first = true;
    for (int i = getSpriteArrayLength() - 1; i >= 0; i--) {
        if (first && isSpriteAnimated(i)) {
            first = false;
            continue;
        }
        if (!first) setSpriteAnimated(false, i);
    }
}
