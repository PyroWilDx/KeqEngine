//
// Created by pyrow on 08/02/2024.
//

#include "Monster/MobDropper.hpp"
#include "Utils/Global.hpp"
#include "World/World.hpp"
#include "World/Background.hpp"
#include "Utils/Utils.hpp"
#include "Utils/Random.hpp"
#include "Monster/Slime.hpp"

MobDropper::MobDropper()
        : Monster(0, 1,
                  MOB_DROPPER_ENUM_N, MOB_DROPPER_DEATH,
                  MOB_DROPPER_FLY, 64) {
    setHitBox({18, 22, 24, 26});
    setRenderWHMultiplier(2., 2.);

    this->currTargetX = TARGET_PADDING_X;
//    this->currTargetY = TARGET_PADDING_Y;

    initSprite(MOB_DROPPER_FLY, "res/gfx/monster/dropper/Fly.png",
               64, 64, 5, 60);
    setXYShift(0, 0, 0, MOB_DROPPER_FLY);

    initSprite(MOB_DROPPER_SPIT, "res/gfx/monster/dropper/Spit.png",
               64, 64, 5, 120);
    setXYShift(0, 0, 0, MOB_DROPPER_SPIT);

    initSprite(MOB_DROPPER_DEATH, "res/gfx/monster/dropper/Death.png",
               64, 64, 8, 60);
    setXYShift(0, 0, 0, MOB_DROPPER_DEATH);

    setSpriteAnimated(true, MOB_DROPPER_FLY);
}

void MobDropper::flyToTarget() {
    xVelocity = 0.2;
}

void MobDropper::spitMob() {
    xVelocity = 0;

    if (isNewestFrame(2, MOB_DROPPER_SPIT)) {
        auto *slime = new Slime("Red");
        slime->moveToEntityCenterFront(this, false);
        slime->setFacingEast(!isFacingEast());
        Global::gWorld->addMonster(slime);
    }
}

void MobDropper::AI() {
    setSpriteAnimated(true, MOB_DROPPER_FLY);

    double minTargetX = Global::gWorld->getBackground()->getFrame().x + TARGET_PADDING_X;
    double maxTargetX = minTargetX + SCREEN_BASE_WIDTH - (2 * TARGET_PADDING_X)
                        - (getRenderWMultiplier() * frameW);
//    double minTargetY = Global::gWorld->getBackground()->getFrame().y + TARGET_PADDING_Y;
//    double maxTargetY = minTargetX + SCREEN_BASE_HEIGHT - 2 * TARGET_PADDING_X;
    if ((isFacingEast() && x > currTargetX - 1.) || (!isFacingEast() && x < currTargetX + 1.)
        || (currTargetX < minTargetX || currTargetX > maxTargetX)) {
        currTargetX = Random::getRandomReal(minTargetX, maxTargetX);
        setFacingEast(currTargetX > getX());
    }

    if (Random::getRandomReal() < 0.0026) {
        setSpriteAnimated(true, MOB_DROPPER_SPIT);
    }
}

int MobDropper::isInvincible() {
    return INVINCIBLE_ALL;
}

bool MobDropper::animDeath() {
    if (willFrameFinish(-1, MOB_DROPPER_DEATH)) {
        Global::gWorld->removeMonster(this);
        return true;
    }
    return false;
}

void MobDropper::updateAction() {
    Monster::updateAction();

    if (isSpriteAnimated(MOB_DROPPER_FLY)) this->flyToTarget();
    if (isSpriteAnimated(MOB_DROPPER_SPIT)) this->spitMob();
}
