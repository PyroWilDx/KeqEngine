//
// Created by pyrowildx on 30/05/23.
//

#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "WindowRenderer.hpp"
#include "Utils.hpp"
#include "AnimatedEntity.hpp"
#include "Keqing.hpp"
#include "Background.hpp"

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
const double MONSTER_WALK_SPEED = 0.2f;
const double MONSTER_RUN_SPEED = 0.6f;

class Monster : public AnimatedEntity {

public:
    explicit Monster(WindowRenderer *window);

    ~Monster() override = default;

    void move(int dt) override;

    void walk();

    void run();

    void attack();

    Monster *copy(WindowRenderer *window);

    inline bool isMoving() {
        return (spriteArray[ZOMBIE_WALK_SPRITE].sAnimated ||
                spriteArray[ZOMBIE_RUN_SPRITE].sAnimated);
    }

    inline bool isAttacking() { return spriteArray[ZOMBIE_ATTACK_SPRITE].sAnimated; }

    inline static bool render(Monster *monster, void *params, void *retVal) {
        WindowRenderer *window;
        Background *background;
        if (sizeof(WindowRenderer) < sizeof(Background)) {
            auto *bParams = (Background *) params;
            window = (WindowRenderer *) &bParams[0];
            background = &bParams[1];
        } else {
            auto *wParams = (WindowRenderer *) params;
            window = &wParams[0];
            background = (Background *) &wParams[1];
        }
        window->render(monster, background);
        return false;
    }

    inline static bool collides(Monster *monster, void *params, void *retVal) {
        Keqing *kq = Keqing::getInstance();
        SDL_Rect addRect;
        if (kq->isNAtking()) addRect = {0, 0, 60, 0}; // TODO CHANGE REACH CURRENT
        else addRect = {0, 0, 0, 0};
        bool collided = kq->collides(monster, addRect);
        bool res = false;
        if (collided) {
            if (kq->isNAtking()) {
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
                     (monster->spriteArray[ZOMBIE_WALK_SPRITE].sAnimated ||
                      monster->spriteArray[ZOMBIE_RUN_SPRITE].sAnimated);
        return false;
    }

    inline static bool isAttacking(Monster *monster, void *params, void *retVal) {
        bool *tmpRetVal = (bool *) retVal;
        *tmpRetVal = *tmpRetVal ||
                     monster->spriteArray[ZOMBIE_ATTACK_SPRITE].sAnimated;
        return false;
    }

private:
    int hp;
};

#endif
