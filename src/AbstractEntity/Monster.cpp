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

        if (doAI) {
            AI();
        } else {
            setSpriteAnimated(true, 0); // Idle
        }

        updateAction();

        moveX();
        moveY();
    }

    animateSprite();

    return doNext;
}

void Monster::onDeath() {
    Global::gWorld->removeMonster(this);
}
