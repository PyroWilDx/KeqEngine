//
// Created by pyrowildx on 30/05/23.
//

#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "WindowRenderer.hpp"
#include "Utils.hpp"
#include "AnimatedEntity.hpp"

enum {
    ZOMBIE_IDLE_SPRITE = 0,
    ZOMBIE_WALK_SPRITE = 1,
    ZOMBIE_RUN_SPRITE = 2,
    ZOMBIE_ATTACK_SPRITE = 3,
    ZOMBIE_HURT_SPRITE = 4,
    ZOMBIE_END_SPRITE_ENUM
};

const int MONSTER_WIDTH = 200;
const int MONSTER_HEIGHT = 200;
const float MONSTER_SPEED = 0.8f;

class Monster : public AnimatedEntity {

public:
    Monster(int w, int h, WindowRenderer window);

    void move(int dt);

    void attack();

    void destroy() override;

private:
    int hp;
};

#endif
