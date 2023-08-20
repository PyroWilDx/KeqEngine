//
// Created by pyrowildx on 24/07/2023.
//

#ifndef LIVINGENTITY_HPP
#define LIVINGENTITY_HPP

#include "AnimatedEntity.hpp"

class LivingEntity : public AnimatedEntity {

public:
    LivingEntity(double gravityWeight, int baseHp,
                 int spriteArrayLength, int hurtSpriteCode,
                 int stateChangerEndSpriteCode);

    ~LivingEntity() override;

    void setXYShift(int xShift, int yShift, int xRShift, int spriteCode);

    void fallGravity() override;

    SDL_Rect getRenderRect() override;

    void onGameFrame() override;

    void setDmgFacingEast(double kbVX);

    void damageSelf(int damage, double kbVX, double kbVY);

    virtual void hurt() = 0;

    virtual void updateAction() = 0;

    [[nodiscard]] inline bool isHurt() { return isSpriteAnimated(hurtSpriteCode); };

    [[nodiscard]] inline int getHp() const { return hp; }

protected:
    int hp;
    int hurtSpriteCode;
    double hurtKbVX;
    double hurtKbVY;
    int timeSinceHurt;
    int stateChangerEndSpriteCode;
    int *xShifts, *yShifts, *xRShifts;

};

#endif
