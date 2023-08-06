//
// Created by pyrowildx on 13/05/23.
//

#ifndef KEQING_HPP
#define KEQING_HPP

#include "WindowRenderer.hpp"
#include "Utils/Utils.hpp"
#include "EntityBase/LivingEntity.hpp"

enum {
    KQ_IDLE = 0,
    KQ_RUN_END,
    KQ_JUMP_END,
    KQ_WALK,
    KQ_WALK_TURN,
    KQ_RUN_START,
    KQ_RUN,
    KQ_RUN_TURN,
    KQ_CROUCH,
    KQ_JUMP_START,
    KQ_JUMP,
    KQ_NATK,
    KQ_CATK,
    KQ_UP_NATK,
    KQ_UP_CATK,
    KQ_CROUCH_NATK,
    KQ_CROUCH_CATK,
    KQ_DASH,
    KQ_SKILL,
    KQ_SKILL_AIMING,
    KQ_SKILL_SLASH,
    KQ_BURST,
    KQ_AIR_DOUBLE_JUMP,
    KQ_AIR_NATK,
    KQ_AIR_UP_NATK,
    KQ_AIR_PLUNGE,
    KQ_AIR_DASH,
    KQ_AIR_SKILL_SLASH,
    KQ_HURT,
    KQ_ENUM_N
};

#define KQ_BASE_COLOR 2962947839

#define KQ_WIDTH_MULTIPLIER 1.0
#define KQ_HEIGHT_MULTIPLIER 1.0

#define KQ_WALK_VELOCITY 0.4
#define KQ_RUN_VELOCITY 0.6
#define KQ_AIR_WALK_VELOCITY 0.38
#define KQ_DASH_VELOCITY 0.8

#define KQ_JUMP_BASE_VELOCITY 1.0
#define KQ_AIR_DOUBLE_JUMP_BASE_VELOCITY 0.8
#define KQ_AIR_DASH_VELOCITY 0.8

#define KQ_KNOCKBACK_VELOCITY 0.6

#define KQ_BURST_NUMBER_OF_CLONE_SLASH 6
#define KQ_BURST_NUMBER_OF_SLASH 8
#define KQ_BURST_NUMBER_OF_CLONE 5

#define KQ_SKILL_COOLDOWN 7000
#define KQ_LIGHTNING_STILETTO_DURATION 6000
#define KQ_BURST_COOLDOWN 10000
#define KQ_SKILL_CIRCLE_RGBA 10, 255, 10, 255

class Keqing : public LivingEntity {

public:
    static void initKeqing();

    static inline Keqing *getInstance() { return instance; }

    static void cleanUp();

    void reset();

    void colorTexture(Uint32 rgba);

    void moveX() override;

    void setSpriteAnimated(bool animated, int spriteCode);

    bool shouldUpdateDirection();

    void updateDirection();

    bool canMoveLR();

    void moveLR();

    void airAnimate();

    void run();

    void crouch();

    void jump();

    void NAtk();

    void CAtk();

    void upNAtk();

    void upCAtk();

    void crouchNAtk();

    void crouchCAtk();

    void dash();

    void ESkill();

    void ESkillAiming();

    void moveToLStiletto();

    void ESkillSlash();

    void RBurst();

    void airDoubleJump();

    void airNAtk();

    void airUpNAtk();

    void airPlunge();

    void airDash();

    void onGameFrame() override;

    void hurt() override;

    void setFacingEast(bool value);

    bool canDoAction(int spriteCode);

    static void preAction(int spriteCode, void *fParams);

    void updateActionFromKey();

    void updateAction() override;

    static inline bool isLightningStilettoExisting() {
        return (Particle::getParticle(PARTICLE_KQ_SKILL_IDLE) != nullptr);
    }

private:
    explicit Keqing();

    static Keqing *instance;

    int jumpPressTime;
    bool doubleJumped;
    bool airDashed;

};

#endif
