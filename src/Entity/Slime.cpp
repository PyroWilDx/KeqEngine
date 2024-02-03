//
// Created by pyrowildx on 24/07/2023.
//

#include "Entity/Slime.hpp"
#include "Keqing.hpp"
#include "Utils/Global.hpp"
#include "EntityInfo/Attack.hpp"
#include "World/World.hpp"
#include "Utils/Utils.hpp"

double Slime::kqMaxDist = 600.;
double Slime::kqAtkDist = 90.;

int Slime::jumpCd = 300;
int Slime::atkCd = 600;

Slime::Slime(const std::string &colorString) :
        Monster(0.002, 1, SLIME_ENUM_N,
                SLIME_DEATH, SLIME_JUMP) {
    setHitBox({0, 0, 16, 16});

    this->lastJumpTime = 0;
    this->lastAtkTime = 0;

    std::string pathStart = "res/gfx/slime/" + colorString;
    initSprite(SLIME_IDLE, (pathStart + "Idle.png").c_str(),
               16, 16, 5, 60);
    setSpriteNext(SLIME_IDLE, SLIME_IDLE);

    initSprite(SLIME_WALK, (pathStart + "Walk.png").c_str(),
               16, 16, 7, 60);
    setSpriteNext(SLIME_WALK, SLIME_WALK);

    initSprite(SLIME_JUMP, (pathStart + "Jump.png").c_str(),
               32, 32, 10, 76);
    setXYShift(-8, -16, -8, SLIME_JUMP);

    initSprite(SLIME_ATK, (pathStart + "Atk.png").c_str(),
               32, 32, 5, 60);
    setXYShift(-8, -16, -8, SLIME_ATK);
    setSpriteFrameLengthFromTo(120, 0, 2, SLIME_ATK);
    setSpriteFrameLengthFromTo(600, 4, 4, SLIME_ATK);

    initSprite(SLIME_DEATH, (pathStart + "Death.png").c_str(),
               16, 16, 5, 60);
    setSpriteFrameLengthFromTo(INT32_MAX, -1, -1, SLIME_DEATH);
}

void Slime::jump() {
    if (isNewestFrame(2, SLIME_JUMP)) {
        yVelocity = -0.46;
    }

    if (isFrameBetween(2, 7, SLIME_JUMP)) {
        xVelocity = 0.16;
    }

    if (willFrameFinish(-1, SLIME_JUMP)) {
        lastJumpTime = Global::currentTime;
    }
}

void Slime::attack() {
    if (isNewestFrame(3, SLIME_ATK)) {
        int N = 40;
        const int HalfPolyN = N / 2 + 1;
        double xShift = 24;
        if (!isFacingEast()) xShift = -xShift;
        double atkPolyPts[N][2];
        approxEllipse(atkPolyPts, N, xShift, 52, 68, 80);

        double atkPolyPtsUp[HalfPolyN][2];
        cutEllipseHalfHorzt(atkPolyPts, N, HalfPolyN,
                            atkPolyPtsUp, nullptr);

        Attack *atk =
                Global::currentWorld->addMonsterAtk(this, atkPolyPtsUp, HalfPolyN,
                                                    10, 1.0, -0.4);
        atk->setShouldRemove([](Attack *atk, void *fParams) {
            return !atk->getAtkIssuer()->isSpriteAnimated(SLIME_ATK);
        }, nullptr);
    }

    if (willFrameFinish(-1, SLIME_ATK)) {
        lastAtkTime = Global::currentTime;
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
    if (isHurt()) return;

    Keqing *kq = Keqing::getInstance();
    double kqDist = distTo(kq);
    if (kqDist < kqAtkDist && Global::currentTime - lastAtkTime > atkCd) {
        setFacingEast(x < kq->getX());
        setSpriteAnimated(true, SLIME_ATK);
    } else if (kqDist < kqMaxDist && Global::currentTime - lastJumpTime > jumpCd) {
        if (!isSpriteAnimated(SLIME_JUMP)) {
            setFacingEast(x < kq->getX());
            setSpriteAnimated(true, SLIME_JUMP);
        }
    } else {
        setSpriteAnimated(true, SLIME_IDLE);
    }

    if (!isInAir()) { // Can Only Move When Jumping
        xVelocity = 0;
    }
}

void Slime::hurt() {
    LivingEntity::hurt();
}

void Slime::updateAction() {
    LivingEntity::updateAction();

    bool first = true;
    for (int i = SLIME_ENUM_N - 1; i >= 0; i--) {
        if (first && isSpriteAnimated(i)) {
            first = false;
            continue;
        }
        if (!first) setSpriteAnimated(false, i);
    }

    if (isSpriteAnimated(SLIME_JUMP)) this->jump();
    if (isSpriteAnimated(SLIME_ATK)) this->attack();
}
