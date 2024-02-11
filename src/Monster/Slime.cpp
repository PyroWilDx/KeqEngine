//
// Created by pyrowildx on 24/07/2023.
//

#include "Monster/Slime.hpp"
#include "Keqing.hpp"
#include "Utils/Global.hpp"
#include "EntityInfo/Attack.hpp"
#include "World/World.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Random.hpp"

Slime::Slime(const std::string &colorString)
        : Monster(0.002, 10000000,
                  SLIME_ENUM_N, SLIME_DEATH,
                  SLIME_JUMP, 32) {
    setHitBox({1, 4, 14, 12});
    setRenderWHMultiplier(4., 4.);

    this->currJumpXVelocity = 0;
    this->lastJumpTime = 0;
    this->currJumpCd = 0;
    this->lastAtkTime = 0;

    std::string pathStart = "res/gfx/monster/slime/" + colorString;
    initSprite(SLIME_IDLE, (pathStart + "Idle.png").c_str(),
               32, 32, 5, 60);
    setXYShift(-8, -16, -8, SLIME_IDLE);
    setSpriteNext(SLIME_IDLE, SLIME_IDLE);

    initSprite(SLIME_JUMP, (pathStart + "Jump.png").c_str(),
               32, 32, 10, 76);
    setXYShift(-8, -16, -8, SLIME_JUMP);

    initSprite(SLIME_ATK, (pathStart + "Atk.png").c_str(),
               32, 32, 5, 60);
    setXYShift(-8, -16, -8, SLIME_ATK);
    setSpriteFrameLengthFromTo(120, 0, 2, SLIME_ATK);
    setSpriteFrameLengthFromTo(600, 4, 4, SLIME_ATK);

    initSprite(SLIME_DEATH, (pathStart + "Death.png").c_str(),
               32, 32, 5, 60);
    setXYShift(-8, -16, -8, SLIME_DEATH);
    setSpriteFrameLengthFromTo(INT32_MAX, 2, 2, SLIME_DEATH);

    setSpriteAnimated(true, SLIME_IDLE);
}

void Slime::jump() {
    if (isNewestFrame(0, SLIME_JUMP)) {
        currJumpXVelocity = Random::getRandomReal(0.126, 0.242);
    } else if (isNewestFrame(2, SLIME_JUMP)) {
        yVelocity = -0.46;
    }

    if (isFrameBetween(2, 7, SLIME_JUMP)) {
        xVelocity = currJumpXVelocity;
    }

    if (willFrameFinish(-1, SLIME_JUMP)) {
        lastJumpTime = Global::currTime;
        currJumpCd = Random::getRandomInt(100, 420);
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
        cutEllipseHalfHorzt(atkPolyPts, HalfPolyN,
                            atkPolyPtsUp, nullptr);

        Attack *atk =
                Global::gWorld->addMonsterAtk(this, atkPolyPtsUp, HalfPolyN,
                                              100, 1.0, -0.4);
        atk->setShouldRemove([](Attack *atk, void *fParams) {
            return !atk->getAtkIssuer()->isSpriteAnimated(SLIME_ATK);
        }, nullptr);
    }

    if (willFrameFinish(-1, SLIME_ATK)) {
        lastAtkTime = Global::currTime;
    }
}

void Slime::AI() {
    if (isHurt()) return;

    setSpriteAnimated(true, SLIME_IDLE);

    Keqing *kq = Keqing::getInstance();
    double kqDist = distTo(kq);
    if (!isInAir()) {
        if (kqDist < SLIME_ATK_DIST && Global::currTime - lastAtkTime > SLIME_ATK_CD) {
            setFacingEast(x < kq->getX());
            setSpriteAnimated(true, SLIME_ATK);
        } else if (kqDist < SLIME_MAX_DIST && Global::currTime - lastJumpTime > currJumpCd) {
            if (!isSpriteAnimated(SLIME_JUMP)) {
                setFacingEast(x < kq->getX());
                setSpriteAnimated(true, SLIME_JUMP);
            }
        }
    }

    if (!isInAir()) { // Can Only Move When Jumping
        xVelocity = 0;
    }
}

int Slime::isInvincible() {
    if (!doAI) {
        return INVINCIBLE_DAMAGE;
    }
    return Monster::isInvincible();
}

void Slime::hurt() {
    Monster::hurt();
}

bool Slime::onDeath() {
    Monster::onDeath();

    setSpriteFrameLengthFromTo(80, 0, -1, SLIME_DEATH);
    goToFrame(0, SLIME_DEATH);
    return false;
}

bool Slime::animDeath() {
    if (willFrameFinish(-1, SLIME_DEATH)) {
        Global::gWorld->removeMonster(this);
        return true;
    }
    return false;
}

void Slime::updateAction() {
    Monster::updateAction();

    if (isSpriteAnimated(SLIME_JUMP)) this->jump();
    if (isSpriteAnimated(SLIME_ATK)) this->attack();
}
