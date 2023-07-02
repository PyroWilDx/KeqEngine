//
// Created by pyrowildx on 13/05/23.
//

#ifndef KEQING_HPP
#define KEQING_HPP

#include "WindowRenderer.hpp"
#include "Utils.hpp"
#include "AnimatedEntity.hpp"

enum {
    KQ_IDLE = 0,
    KQ_JUMP_END,
    KQ_WALK,
    KQ_TURN,
    KQ_NATK, // NAtk = Normal Attack
    KQ_DASH,
    KQ_SKILL,
    KQ_SKILL_SLASH,
    KQ_BURST,
    KQ_JUMP_START,
    KQ_JUMP,
    KQ_AIR_DASH,
    KQ_AIR_NATK,
    KQ_HURT,
    KQ_ENUM_N
};

#define KQ_WIDTH_MULTIPLIER 1.0
#define KQ_HEIGHT_MULTIPLIER 1.0

#define KQ_WALK_VELOCITY 0.4
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

    SDL_Rect getRenderRect() override;

    void updateDirection();

    void NAtk();

    void dash();

    void ESkill();

    void ESkillSlash();

    void RBurst();

    void airAnimate();

    void jump();

    void airNAtk();

    void airDash();

    void damage();

    void setFacingEast(bool value);

    bool canDoAction(int spriteCode);

    void preAction(int spriteCode);

    inline bool isNAtking() { return (spriteArray[KQ_NATK].sAnimated); } // TODO CHANGE TO JUSTE UNE FONTION UPDATE

    inline bool isDashing() { return (spriteArray[KQ_DASH].sAnimated); }

    inline bool isESkilling() { return (spriteArray[KQ_SKILL].sAnimated); }

    inline bool isESkillSlashing() { return (spriteArray[KQ_SKILL_SLASH].sAnimated); }

    inline bool isRBursting() { return (spriteArray[KQ_BURST].sAnimated); }

    inline bool isJumping() { return (spriteArray[KQ_JUMP].sAnimated); }

    inline bool isAirNAtking() { return (spriteArray[KQ_AIR_NATK].sAnimated); }

    inline bool isAirDashing() { return (spriteArray[KQ_AIR_DASH].sAnimated); }

    inline bool isDamaged() { return (spriteArray[KQ_HURT].sAnimated); }

    inline bool isMoving() { return (xVelocity != 0); }

    static inline bool isLightningStilettoExisting() {
        return (Particle::getParticle(PARTICLE_KQ_SKILL_IDLE, 0) != nullptr);
    }

    // TODO may need isInvincible

    //TODO MAYBE IGNORE LAST FRAME TO MORE RESPONSIVITY

    [[nodiscard]] inline int getHp() const { return hp; }

private:
    explicit Keqing();

    static Keqing *instance;
    int hp;
};

#endif
