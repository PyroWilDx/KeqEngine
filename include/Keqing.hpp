//
// Created by pyrowildx on 13/05/23.
//

#ifndef KEQING_HPP
#define KEQING_HPP

#include "WindowRenderer.hpp"
#include "Utils.hpp"
#include "AnimatedEntity.hpp"

enum {
    KQ_IDLE_SPRITE = 0,
    KQ_JUMP_LAND_SPRITE,
    KQ_DASH_STOP_SPRITE,
    KQ_WALK_SPRITE,
    KQ_TURN_SPRITE,
    KQ_NATTACKS_SPRITE, // NAttacks = Normal Attacks
    KQ_DASH_START_SPRITE,
    KQ_DASH_SPRITE,
    KQ_TURN_DASH_SPRITE,
    KQ_STELLAR_RESTORATION_SPRITE,
    KQ_STELLAR_RESTORATION_SLASH_SPRITE,
    KQ_STARWARD_SWORD_SPRITE,
    KQ_JUMP_START_SPRITE,
    KQ_JUMP_SPRITE,
    KQ_AIR_DASH_SPRITE,
    KQ_AIR_NATTACK_SPRITE,
    KQ_HURT_SPRITE,
    KQ_END_SPRITE_ENUM
};

#define KQ_WIDTH_MULTIPLIER 1.4f
#define KQ_HEIGHT_MULTIPLIER 1.4f

#define KQ_WALK_VELOCITY 0.4f
#define KQ_DASH_VELOCITY 1.0f
#define KQ_Z_VELOCITY 0.24f

#define KQ_BASE_JUMP_VELOCITY 1.0f
#define KQ_AIR_DASH_VELOCITY 0.8f

#define KQ_KNOCKBACK_VELOCITY 0.6f

#define KQ_SS_NUMBER_OF_CLONE_SLASH 6
#define KQ_SS_NUMBER_OF_SLASH 8

class Keqing : public AnimatedEntity {

public:
    static void initKeqing(WindowRenderer *window);

    static inline Keqing *getInstance() { return instance; }

    void updateDirection(const bool *pressedKeys, int lastKey);

    void move(int dt) override;

    void nattack(int dt, int currentTime);

    void dash();

    void stellarRestoration();

    void stellarRestorationSlash();

    void starwardSword(int dt);

    void jump(int dt);

    void airNAttack(int dt);

    void airDash();

    void damage(int dt);

    void setFacingEast(bool value);

    bool canDoAction(int spriteCode);

    bool shouldNotMove();

    void preAction(int spriteCode);

    void destroy() override;

    inline bool isNAttacking() { return spriteArray[KQ_NATTACKS_SPRITE].animated; }

    inline bool isDashing() {
        return (spriteArray[KQ_DASH_START_SPRITE].animated ||
                spriteArray[KQ_DASH_SPRITE].animated ||
                spriteArray[KQ_DASH_STOP_SPRITE].animated);
    }

    inline bool isESkilling() { return spriteArray[KQ_STELLAR_RESTORATION_SPRITE].animated; }

    inline bool isESlashing() { return spriteArray[KQ_STELLAR_RESTORATION_SLASH_SPRITE].animated; }

    inline bool isRBursting() { return spriteArray[KQ_STARWARD_SWORD_SPRITE].animated; }

    inline bool isJumping() { return spriteArray[KQ_JUMP_SPRITE].animated; }

    inline bool isAirNAttacking() { return spriteArray[KQ_AIR_NATTACK_SPRITE].animated; }

    inline bool isAirDashing() { return spriteArray[KQ_AIR_DASH_SPRITE].animated; }

    inline bool isDamaged() { return spriteArray[KQ_HURT_SPRITE].animated; }

    inline bool isMoving() { return (xVelocity != 0 || zVelocity != 0); }

    // TODO may need isInvincible

    inline int getHp() { return hp; }

private:
    explicit Keqing(WindowRenderer *window);

    static Keqing *instance;
    int hp;
    int lastNAttackTime;
    float dashXToAdd;
};

#endif
