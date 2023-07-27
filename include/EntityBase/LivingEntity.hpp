//
// Created by pyrow on 24/07/2023.
//

#ifndef LIVINGENTITY_HPP
#define LIVINGENTITY_HPP

#include "AnimatedEntity.hpp"

class LivingEntity : public AnimatedEntity {

public:
    LivingEntity(double gravityWeight, int spriteArrayLength,
                 int baseHp, int hurtSpriteCode);

    ~LivingEntity() override;

    SDL_Rect getRenderRect() override;

    void damageSelf(int damage, double kbVX, double kbVY);

    virtual void hurt() = 0;

    void setXYShift(int xShift, int yShift, int xRShift, int spriteCode);

    [[nodiscard]] inline int getHp() const { return hp; }

protected:
    int hp;
    int hurtSpriteCode;
    double hurtKbVX;
    double hurtKbVY;
    int hurtStartTime;
    int *xShifts, *yShifts, *xRShifts;

};

#endif
