//
// Created by pyrow on 08/02/2024.
//

#include "Monster/MobDropper.hpp"

MobDropper::MobDropper()
        : Monster(0, 1, MOB_DROPPER_ENUM_N,
                  MOB_DROPPER_FLY, MOB_DROPPER_FLY) {
    setHitBox({0, 0, 0, 0});

    initSprite(MOB_DROPPER_FLY, "",
               32, 32, 5, 60);
    setXYShift(-8, -16, -8, MOB_DROPPER_FLY);

    setSpriteAnimated(true, MOB_DROPPER_FLY);
}

void MobDropper::AI() {

}

int MobDropper::isInvincible() {
    return INVINCIBLE_ALL;
}
