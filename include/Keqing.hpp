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
//    KQ_DASH_END,
//    KQ_SKILL_END,
//    KQ_SKILL_SLASH_END,
//    KQ_BURST_END,
    KQ_WALK,
    KQ_TURN,
    KQ_NATK, // NAtk = Normal Attack
//    KQ_DASH_START,
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

#define KQ_WIDTH_MULTIPLIER 1.4f
#define KQ_HEIGHT_MULTIPLIER 1.4f

#define KQ_WALK_VELOCITY 0.4f
#define KQ_DASH_VELOCITY 1.0f

#define KQ_BASE_JUMP_VELOCITY 1.0f
#define KQ_AIR_DASH_VELOCITY 0.8f

#define KQ_KNOCKBACK_VELOCITY 0.6f

#define KQ_BURST_NUMBER_OF_CLONE_SLASH 6
#define KQ_BURST_NUMBER_OF_SLASH 8
#define KQ_BURST_NUMBER_OF_CLONE 5

#define KQ_SKILL_COOLDOWN 7000
#define KQ_LIGHTNING_STILETTO_DURATION 6000
#define KQ_BURST_COOLDOWN 10000
#define KQ_SKILL_CIRCLE_RGBA 10, 255, 10, 255

class Keqing : public AnimatedEntity {

public:
    static void initKeqing(WindowRenderer *window);

    static inline Keqing *getInstance() { return instance; }

    void colorTexture(int r, int g, int b, WindowRenderer *window);

    void updateDirection(const bool *pressedKeys, int lastKey);

    void move(int dt) override;

    void nAtk(int dt, int currentTime);

    void dash(bool *pressedKeys);

    void skill();

    void skillSlash();

    void burst(int dt);

    void jump(int dt);

    void airNAtk(int dt);

    void airDash();

    void damage(int dt);

    void setFacingEast(bool value);

    bool canDoAction(int spriteCode);

    bool shouldNotMove();

    void preAction(int spriteCode, const bool *pressedKeys);

    void destroy() override;

    inline bool isNAtking() { return (spriteArray[KQ_NATK].animated); }

    inline bool isDashing() { return (spriteArray[KQ_DASH].animated); }

    inline bool isSkilling() { return (spriteArray[KQ_SKILL].animated); }

    inline bool isSkillSlashing() { return (spriteArray[KQ_SKILL_SLASH].animated); }

    inline bool isBursting() { return (spriteArray[KQ_BURST].animated); }

    inline bool isJumping() { return (spriteArray[KQ_JUMP].animated); }

    inline bool isAirNAtking() { return (spriteArray[KQ_AIR_NATK].animated); }

    inline bool isAirDashing() { return (spriteArray[KQ_AIR_DASH].animated); }

    inline bool isDamaged() { return (spriteArray[KQ_HURT].animated); }

    inline bool isMoving() { return (xVelocity != 0); }

    static inline bool isLightningStilettoExisting() { return (Particle::getParticle(PARTICLE_KQ_SKILL_IDLE, 0) != nullptr); }

    // TODO may need isInvincible

    [[nodiscard]] inline int getHp() const { return hp; }

private:
    explicit Keqing(WindowRenderer *window);

    static Keqing *instance;
    int hp;
    int lastNAttackTime; // TODO
};

#endif
