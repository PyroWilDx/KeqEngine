//
// Created by pyrowildx on 24/07/2023.
//

#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "LivingEntity.hpp"

class Monster : public LivingEntity {

    using LivingEntity::LivingEntity;

public:
    Monster(double gravityWeight, int baseHp,
            int spriteArrayLength, int hurtSpriteCode,
            int stateChangerEndSpriteCode);

    bool onGameFrame() override;

    virtual void AI() = 0;

    void onDeath() override;

    void setDoAI(bool doAI_) { doAI = doAI_; }

protected:
    bool doAI;

};

#endif
