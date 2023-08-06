//
// Created by pyrow on 24/07/2023.
//

#include "Entity/Slime.hpp"
#include "Entity/Keqing.hpp"
#include "Utils/Global.hpp"
#include "SDL2_gfx/SDL2_gfxPrimitives.h"

Slime::Slime(const std::string &colorString) :
        Monster(0.002, 1, SLIME_ENUM_N,
                SLIME_DEATH, SLIME_JUMP) {
    hitBox = {0, 0, 16, 16};

    std::string pathStart = "res/gfx/slime/" + colorString;
    initSprite(SLIME_IDLE, (pathStart + "Idle.png").c_str(),
               16, 16, 5, 60);
    setSpriteNext(SLIME_IDLE, SLIME_IDLE);

    initSprite(SLIME_WALK, (pathStart + "Walk.png").c_str(),
               16, 16, 7, 60);
    setSpriteNext(SLIME_WALK, SLIME_WALK);

    initSprite(SLIME_CROUCH, (pathStart + "Crouch.png").c_str(),
               16, 16, 3, 60);

    initSprite(SLIME_JUMP, (pathStart + "Jump.png").c_str(),
               32, 32, 10, 60);
    setXYShift(-8, -16, -8, SLIME_JUMP);

    initSprite(SLIME_ATK, (pathStart + "Atk.png").c_str(),
               32, 32, 5, 60);
    setXYShift(-8, -16, -8, SLIME_ATK);
    setSpriteFrameLengthFromTo(100, 0, 2, SLIME_ATK);
    setSpriteFrameLengthFromTo(INT32_MAX, 4, 4, SLIME_ATK);

    initSprite(SLIME_DEATH, (pathStart + "Death.png").c_str(),
               16, 16, 5, 60);
    setSpriteFrameLengthFromTo(INT32_MAX, -1, -1, SLIME_DEATH);
}

void Slime::onGameFrame() {
    Monster::onGameFrame();

    fallGravity();

    AI();

    updateAction();

    moveX();
    moveY();

    animateSprite();
}

void Slime::AI() {
    setSpriteAnimated(true, SLIME_IDLE);

    Keqing *kq = Keqing::getInstance();
    if (abs(kq->getX() - x) < 110) {
        setSpriteAnimated(true, SLIME_ATK);
    }
    double xyArray[4][2] = {{-10,                    -10},
                            {(double) hitBox.w + 10, -10},
                            {(double) hitBox.w + 10, (double) hitBox.h + 10},
                            {-10,                    (double) hitBox.h + 10}};
    if (isSpriteAnimated(SLIME_ATK)) {
        if (isNewestFrame(0, SLIME_ATK)) {
            Attack *atk =
                    Global::currentWorld->addMonsterAtk(this, xyArray, 4,
                                                        10, 1.0, -0.4);
            atk->setAtkDuration(INT32_MAX);
        }
    }
}

void Slime::hurt() {
    LivingEntity::hurt();
}

void Slime::updateAction() {
    LivingEntity::updateAction();

    if (isSpriteAnimated(SLIME_DEATH)) this->hurt();
}
