//
// Created by pyrow on 08/02/2024.
//

#ifndef MOBDROPPER_HPP
#define MOBDROPPER_HPP

#include "AbstractEntity/Monster.hpp"

#define TARGET_PADDING_X 8.
#define TARGET_PADDING_Y 6.

enum {
    MOB_DROPPER_FLY,
    MOB_DROPPER_SPIT,
    MOB_DROPPER_DEATH,
    MOB_DROPPER_ENUM_N
};

class MobDropper : public Monster {

public:
    MobDropper();

    void flyToTarget();

    void spitMob();

    void AI() override;

    int isInvincible() override;

    bool animDeath() override;

    void updateAction() override;

private:
    double currTargetX;
//    double currTargetY;

};

#endif
