//
// Created by pyrowildx on 24/07/2023.
//

#include "AbstractEntity/Monster.hpp"
#include "Utils/Global.hpp"
#include "World/World.hpp"

Monster::Monster(double gravityWeight, int baseHp,
                 int spriteArrayLength, int hurtSpriteCode,
                 int stateChangerEndSpriteCode)
        : LivingEntity(gravityWeight, baseHp,
                       spriteArrayLength, hurtSpriteCode,
                       stateChangerEndSpriteCode) {
    this->doAI = true;
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
