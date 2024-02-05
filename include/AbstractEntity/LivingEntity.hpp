//
// Created by pyrowildx on 24/07/2023.
//

#ifndef LIVINGENTITY_HPP
#define LIVINGENTITY_HPP

#include "AnimatedEntity.hpp"

#define ENTITY_MIN_HURT_LOCK_TIME 60.
#define ENTITY_MAX_HURT_LOCK_TIME 120.
#define ENTITY_DIFF_HURT_LOCK_TIME (ENTITY_MAX_HURT_LOCK_TIME - ENTITY_MIN_HURT_LOCK_TIME)

#define HP_BAR_HEIGHT 8
#define HP_BAR_GAP 8.

enum {
    INVINCIBLE_NONE,
    INVINCIBLE_DAMAGE,
    INVINCIBLE_ALL
};

class LivingEntity : public AnimatedEntity {

public:
    LivingEntity(double gravityWeight, int baseHp,
                 int spriteArrayLength, int hurtSpriteCode,
                 int stateChangerEndSpriteCode);

    ~LivingEntity() override;

    void setXYShift(int xShift, int yShift, int xRShift, int spriteCode);

    SDL_Rect getRenderRect() override;

    bool onGameFrame() override;

    void renderSelf(SDL_Renderer *gRenderer) override;

    void healFull();

    virtual int isInvincible();

    void setDmgFacingEast(double kbXV);

    virtual bool damageSelf(int damage, double kbXV, double kbYV);

    virtual void hurt();

    virtual inline void onDeath() {};

    virtual void updateAction() = 0;

    [[nodiscard]] inline bool isHurt() { return isSpriteAnimated(hurtSpriteCode); };

    [[nodiscard]] inline int getHp() const { return currHp; }

protected:
    int maxHp;
    int currHp;
    bool isDead;
    int hurtSpriteCode;
    double hurtKbXV;
    double hurtKbVY;
    int hitLagTime;
    int timeSinceHurt;
    int stateChangerEndSpriteCode;
    int *xShifts, *yShifts, *xRShifts;

};

#endif
