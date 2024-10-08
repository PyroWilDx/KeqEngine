//
// Created by pyrowildx on 24/07/2023.
//

#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "LivingEntity.hpp"
#include "Inventory/RewardInterface.hpp"

class Monster : public LivingEntity {

public:
    Monster(double gravityWeight, int baseHp,
            int spriteArrayLength, int hurtSpriteCode,
            int stateChangerEndSpriteCode, int frameW);

    ~Monster();

    void moveToEntityCenterFront(Entity *centerEntity, bool takeFaceEast = true);

    bool onGameFrame() override;

    virtual void AI() = 0;

    bool onDeath() override;

    virtual bool animDeath() = 0;

    void updateAction() override;

    void setDoAI(bool doAI_) { doAI = doAI_; }

    void setKillReward(RewardInterface *killReward_) { killReward = killReward_; }

protected:
    int frameW;
    bool doAI;

    RewardInterface *killReward;

};

#endif
