//
// Created by pyrowildx on 13/05/23.
//

#ifndef KEQING_HPP
#define KEQING_HPP

#include "WindowRenderer.hpp"
#include "Utils/Utils.hpp"
#include "EntityBase/AnimatedEntity.hpp"

enum {
    KQ_IDLE = 0,
    KQ_RUN_END,
    KQ_JUMP_END,
    KQ_WALK,
    KQ_WALK_TURN,
    KQ_RUN_START,
    KQ_RUN,
    KQ_RUN_TURN,
    KQ_JUMP_START,
    KQ_JUMP,
    KQ_NATK,
    KQ_CATK,
    KQ_DASH,
    KQ_SKILL,
    KQ_SKILL_SLASH,
    KQ_BURST,
    KQ_AIR_NATK,
    KQ_AIR_DASH,
    KQ_HURT,
    KQ_ENUM_N
};

#define KQ_WIDTH_MULTIPLIER 1.0
#define KQ_HEIGHT_MULTIPLIER 1.0

#define KQ_WALK_VELOCITY 0.4
#define KQ_RUN_VELOCITY 0.6
#define KQ_DASH_VELOCITY 0.8

#define KQ_JUMP_BASE_VELOCITY 1.0
#define KQ_AIR_DASH_VELOCITY 0.8

#define KQ_KNOCKBACK_VELOCITY 0.6

#define KQ_BURST_NUMBER_OF_CLONE_SLASH 6
#define KQ_BURST_NUMBER_OF_SLASH 8
#define KQ_BURST_NUMBER_OF_CLONE 5

#define KQ_SKILL_COOLDOWN 7000
#define KQ_LIGHTNING_STILETTO_DURATION 6000
#define KQ_BURST_COOLDOWN 10000
#define KQ_SKILL_CIRCLE_RGBA 10, 255, 10, 255

class Keqing : public AnimatedEntity {

public:
    ~Keqing() override = default;

    static void initKeqing();

    static inline Keqing *getInstance() { return instance; }

    void colorTexture(int r, int g, int b);

    void moveX() override;

    SDL_Rect getRenderRect() override;

    void setSpriteAnimated(int spriteCode, bool animated) override;

    bool shouldUpdateDirection();

    void updateDirection();

//    bool canWalk();

    bool canMoveLR();

    void moveLR();

//    void walk();
//
//    void run();
//
//    void jumpMove();

    void jump();

    void NAtk();

    void CAtk();

    void dash();

    void ESkill();

    void ESkillSlash();

    void RBurst();

    void airAnimate();

    void airNAtk();

    void airDash();

    void damage();

    void setFacingEast(bool value);

    bool canDoAction(int spriteCode);

    void preAction(int spriteCode);

    void updateAction();

    static inline bool isLightningStilettoExisting() {
        return (Particle::getParticle(PARTICLE_KQ_SKILL_IDLE, 0) != nullptr);
    }

    // TODO may need isInvincible

    [[nodiscard]] inline int getHp() const { return hp; }

private:
    explicit Keqing();

    static Keqing *instance;
    static int spriteXShifts[KQ_ENUM_N];
    static int spriteYShifts[KQ_ENUM_N];
    static int spriteXRShifts[KQ_ENUM_N];

    static void setXYShift(int spriteCode, int xShift, int yShift, int xRShift);

    int hp;
};

#endif
