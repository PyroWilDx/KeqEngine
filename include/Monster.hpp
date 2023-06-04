//
// Created by pyrowildx on 30/05/23.
//

#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "WindowRenderer.hpp"
#include "Utils.hpp"
#include "AnimatedEntity.hpp"
#include "Player.hpp"

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
const float MONSTER_WALK_SPEED = 0.2f;
const float MONSTER_RUN_SPEED = 0.6f;

const int PLAYER_REACH = 50;

class Monster : public AnimatedEntity {

public:
    Monster(int w, int h, WindowRenderer *window);

    void move(int dt) override;

    void walk();

    void run();

    void attack();

    Monster *copy(WindowRenderer *window);

    void destroy() override;

    inline bool isMoving() {
        return (spriteArray[ZOMBIE_WALK_SPRITE].animated ||
                spriteArray[ZOMBIE_RUN_SPRITE].animated);
    }

    inline bool isAttacking() { return spriteArray[ZOMBIE_ATTACK_SPRITE].animated; }

    inline static bool render(Monster *monster, void *params, void *retVal) {
        auto *window = (WindowRenderer *) params;
        window->render(monster);
        return false;
    }

    inline static bool addX(Monster *monster, void *params, void *retVal) {
        int *x_ = (int *) params;
        auto *tmp = (Entity *) monster;
        tmp->addX(*x_);
        return false;
    }

    inline static bool collides(Monster *monster, void *params, void *retVal) {
        auto *player = (Player *) params;
        SDL_Rect addRect;
        if (player->isAttacking()) addRect = {0, 0, PLAYER_REACH, 0};
        else addRect = {0, 0, 0, 0};
        bool collided = player->collides(monster, addRect);
        bool res = false;
        if (collided) {
            if (player->isAttacking()) {
                res = true;
            }
        }
        bool *tmpRetVal = (bool *) retVal;
        *tmpRetVal = *tmpRetVal || (collided && !res);
        return res;
    }

    inline static bool animate(Monster *monster, void *params, void *retVal) {
        int *dt = (int *) params;
        auto *tmp = (AnimatedEntity *) monster;
        tmp->animate(*dt);
        return false;
    }

    inline static bool move(Monster *monster, void *params, void *retVal) {
        int *dt = (int *) params;
        monster->move(*dt);
        return false;
    }

    inline static bool walk(Monster *monster, void *params, void *retVal) {
        monster->walk();
        return false;
    }

    inline static bool run(Monster *monster, void *params, void *retVal) {
        monster->run();
        return false;
    }

    inline static bool attack(Monster *monster, void *params, void *retVal) {
        monster->attack();
        return false;
    }

    inline static bool isMoving(Monster *monster, void *params, void *retVal) {
        bool *tmpRetVal = (bool *) retVal;
        *tmpRetVal = *tmpRetVal ||
                     (monster->spriteArray[ZOMBIE_WALK_SPRITE].animated ||
                      monster->spriteArray[ZOMBIE_RUN_SPRITE].animated);
        return false;
    }

    inline static bool isAttacking(Monster *monster, void *params, void *retVal) {
        bool *tmpRetVal = (bool *) retVal;
        *tmpRetVal = *tmpRetVal ||
                     monster->spriteArray[ZOMBIE_ATTACK_SPRITE].animated;
        return false;
    }

private:
    int hp;
};

#endif
