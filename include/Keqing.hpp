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
    KQ_WALK_SPRITE = 1,
    KQ_JUMP_SPRITE = 2,
    KQ_ATTACK_SPRITE = 3,
    KQ_HURT_SPRITE = 4,
    KQ_TURN_SPRITE = 5,
    KQ_END_SPRITE_ENUM
};

#define KQ_WIDTH_MULTIPLIER 2.0f
#define KQ_HEIGHT_MULTIPLIER 2.0f
#define KQ_SPEED 0.4f
#define KQ_BASE_JUMP_VELOCITY 1.0f
#define KQ_KNOCKBACK_SDEED 0.6f

class Keqing : public AnimatedEntity {

public:
    static void initKeqing(WindowRenderer *window);

    static inline Keqing *getInstance() { return instance; }

    void updateDirection(int key, const bool *keyPressed);

    void clearDirection(int key, const bool *keyPressed);

    void move(int dt) override;

    void jump(int dt);

    void attack(int dt, int currenTime);

    void damage(int dt);

    void destroy() override;

    inline bool isMoving() { return spriteArray[KQ_WALK_SPRITE].animated; }

    inline bool isJumping() { return spriteArray[KQ_JUMP_SPRITE].animated; }

    inline bool isAttacking() { return spriteArray[KQ_ATTACK_SPRITE].animated; }

    inline bool isDamaged() { return spriteArray[KQ_HURT_SPRITE].animated; }

    inline int getHp() { return hp; }

private:
    explicit Keqing(WindowRenderer *window);

    static Keqing *instance;
    int hp;
    float jumpVelocity;
    int attackEndTime;
};

#endif
