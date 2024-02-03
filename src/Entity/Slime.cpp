//
// Created by pyrowildx on 24/07/2023.
//

#include "Entity/Slime.hpp"
#include "Keqing.hpp"
#include "Utils/Global.hpp"
#include "EntityInfo/Attack.hpp"
#include "World/World.hpp"
#include "Utils/Utils.hpp"

Slime::Slime(const std::string &colorString) :
        Monster(0.002, 1, SLIME_ENUM_N,
                SLIME_DEATH, SLIME_JUMP) {
    setHitBox({0, 0, 16, 16});

    std::string pathStart = "res/gfx/slime/" + colorString;
    initSprite(SLIME_IDLE, (pathStart + "Idle.png").c_str(),
               16, 16, 5, 60);
    setSpriteNext(SLIME_IDLE, SLIME_IDLE);

    initSprite(SLIME_WALK, (pathStart + "Walk.png").c_str(),
               16, 16, 7, 60);
    setSpriteNext(SLIME_WALK, SLIME_WALK);

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

void Slime::attack() {
    if (isNewestFrame(3, SLIME_ATK)) {
        int N = 40;
        const int HalfPolyN = N / 2 + 1;
        double atkPolyPts[N][2];
        approxEllipse(atkPolyPts, N, 24, 52, 68, 80);

        double atkPolyPtsUp[HalfPolyN][2];
        cutEllipseHalfHorzt(atkPolyPts, N, HalfPolyN,
                            atkPolyPtsUp, nullptr);

        Attack *atk =
                Global::currentWorld->addMonsterAtk(this, atkPolyPtsUp, HalfPolyN,
                                                    10, 1.0, -0.4);
        atk->setAtkDuration(INT32_MAX);
    }
}

bool Slime::onGameFrame() {
    bool doNext = Monster::onGameFrame();

    if (doNext) {
        fallGravity();

        AI();

        updateAction();

        moveX();
        moveY();
    }

    animateSprite();

    return doNext;
}

void Slime::AI() {
    setSpriteAnimated(true, SLIME_IDLE);

    Keqing *kq = Keqing::getInstance();
    double kqDist = std::abs(kq->getX() - x);
    if (kqDist < 60) {
        setSpriteAnimated(true, SLIME_ATK);
    } else if (kqDist < 600) {
        setSpriteAnimated(true, SLIME_JUMP);
    }
}

void Slime::hurt() {
    LivingEntity::hurt();
}

void Slime::updateAction() {
    LivingEntity::updateAction();

    if (isSpriteAnimated(SLIME_ATK)) this->attack();
}
