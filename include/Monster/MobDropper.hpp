//
// Created by pyrow on 08/02/2024.
//

#ifndef MOBDROPPER_HPP
#define MOBDROPPER_HPP

#include "AbstractEntity/Monster.hpp"

enum {
    MOB_DROPPER_FLY,
    MOB_DROPPER_ENUM_N
};

class MobDropper : Monster {

public:
    MobDropper();

    void AI() override;

    int isInvincible() override;

private:

};

#endif
