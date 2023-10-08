//
// Created by pyrowildx on 24/07/2023.
//

#ifndef LIVINGENTITY_HPP
#define LIVINGENTITY_HPP

#include "AnimatedEntity.hpp"

#define ENTITY_MIN_HURT_LOCK_TIME 60.
#define ENTITY_MAX_HURT_LOCK_TIME 120.
#define ENTITY_DIFF_HURT_LOCK_TIME (ENTITY_MAX_HURT_LOCK_TIME - ENTITY_MIN_HURT_LOCK_TIME)

class LivingEntity : public AnimatedEntity {

public:
    LivingEntity(double gravityWeight, int baseHp,
                 int spriteArrayLength, int hurtSpriteCode,
                 int stateChangerEndSpriteCode);

    ~LivingEntity() override;

    void setXYShift(int xShift, int yShift, int xRShift, int spriteCode);

    SDL_Rect getRenderRect() override;

    bool onGameFrame() override;

    virtual bool isInvincible();

    void setDmgFacingEast(double kbXV);

    bool damageSelf(int damage, double kbXV, double kbVY);

    virtual void hurt() = 0;

    virtual void updateAction() = 0;

    [[nodiscard]] inline bool isHurt() { return isSpriteAnimated(hurtSpriteCode); };

    [[nodiscard]] inline int getHp() const { return hp; }

protected:
    int hp;
    int hurtSpriteCode;
    double hurtKbXV;
    double hurtKbVY;
    int hitLagTime;
    int timeSinceHurt;
    int stateChangerEndSpriteCode;
    int *xShifts, *yShifts, *xRShifts;

};

#endif
