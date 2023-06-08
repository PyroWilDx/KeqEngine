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
    KQ_JUMP_START_SPRITE,
    KQ_JUMP_SPRITE,
    KQ_NATTACKS_SPRITE, // Normal Attacks
    KQ_DASH_START_SPRITE,
    KQ_DASH_SPRITE,
    KQ_TURN_DASH_SPRITE,
    KQ_JUMP_DASH_SPRITE,
    KQ_AIR_NATTACK_SPRITE,
    KQ_HURT_SPRITE,
    KQ_STARWARD_SWORD_SPRITE,
    KQ_END_SPRITE_ENUM
};

#define KQ_WIDTH_MULTIPLIER 1.4f
#define KQ_HEIGHT_MULTIPLIER 1.4f
#define KQ_SPEED 0.4f
#define KQ_BASE_JUMP_VELOCITY 1.0f
#define KQ_DASH_SPEED 1.0f
#define KQ_JUMP_DASH_SPEED 0.68f
#define KQ_KNOCKBACK_SPEED 0.6f

class Keqing : public AnimatedEntity {

public:
    static void initKeqing(WindowRenderer *window);

    static inline Keqing *getInstance() { return instance; }

    void updateDirection(int key, const bool *keyPressed);

    void clearDirection(int key, const bool *keyPressed);

    void move(int dt) override;

    void jump(int dt);

    void nattack(int dt, int currentTime);

    void airNAttack(int dt);

    void stellarRestoration();

    void starwardSword();

    void dash(int dt);

    void jumpDash(int dt);

    void damage(int dt);

    void setFacingEast(bool value);

    void destroy() override;

    inline bool isMoving() { return spriteArray[KQ_WALK_SPRITE].animated; }

    inline bool isJumping() { return spriteArray[KQ_JUMP_SPRITE].animated; }

    inline bool isNAttacking() { return spriteArray[KQ_NATTACKS_SPRITE].animated; }

    inline bool isAirNAttacking() { return spriteArray[KQ_AIR_NATTACK_SPRITE].animated; }

    inline bool isESkilling() { return false; } // TODO

    inline bool isRBursting() { return spriteArray[KQ_STARWARD_SWORD_SPRITE].animated; }

    inline bool isDashing() {
        return (spriteArray[KQ_DASH_START_SPRITE].animated ||
                spriteArray[KQ_DASH_SPRITE].animated ||
                spriteArray[KQ_DASH_STOP_SPRITE].animated);
    }

    inline bool isJumpDashing() { return spriteArray[KQ_JUMP_DASH_SPRITE].animated; }

    inline bool isDamaged() { return spriteArray[KQ_HURT_SPRITE].animated; }

    // TODO may need isInvincible

    inline int getHp() { return hp; }

private:
    explicit Keqing(WindowRenderer *window);

    static Keqing *instance;
    int hp;
    float jumpVelocity;
    int lastNAttackTime;
    float dashXToAdd;
};

#endif
