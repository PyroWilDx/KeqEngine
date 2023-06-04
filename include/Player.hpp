//
// Created by pyrowildx on 13/05/23.
//

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "WindowRenderer.hpp"
#include "Utils.hpp"
#include "AnimatedEntity.hpp"

enum {
    PLAYER_IDLE_SPRITE = 0,
    PLAYER_RUN_SPRITE = 1,
    PLAYER_JUMP_SPRITE = 2,
    PLAYER_ATTACK_SPRITE = 3,
    PLAYER_HURT_SPRITE = 4,
    PLAYER_TURN_SPRITE = 5,
    PLAYER_END_SPRITE_ENUM
};

const int PLAYER_WIDTH = 200;
const int PLAYER_HEIGHT = 200;
const float PLAYER_SPEED = 0.4f;
const float PLAYER_BASE_JUMP_VELOCITY = 1.0f;
const float PLAYER_KNOCKBACK_SDEED = 0.6f;

class Player : public AnimatedEntity {

public:
    Player(int w, int h, WindowRenderer *window);

    void updateDirection(int key, const bool *keyPressed);

    void clearDirection(int key, const bool *keyPressed);

    void move(int dt) override;

    void jump(int dt);

    void attack();

    void damage(int dt);

    void destroy() override;

    inline bool isMoving() { return spriteArray[PLAYER_RUN_SPRITE].animated; }

    inline bool isJumping() { return spriteArray[PLAYER_JUMP_SPRITE].animated; }

    inline bool isAttacking() { return spriteArray[PLAYER_ATTACK_SPRITE].animated; }

    inline bool isDamaged() { return spriteArray[PLAYER_HURT_SPRITE].animated; }

    inline int getHp() { return hp; }

private:
    int hp;
    float jumpVelocity;
};

#endif
