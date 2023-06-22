//
// Created by pyrowildx on 30/05/23.
//

#include "Monster.hpp"

Monster::Monster(WindowRenderer *window)
        : AnimatedEntity(ZOMBIE_END_SPRITE_ENUM) {
    xVelocity = MONSTER_WALK_SPEED;
    hp = 1;

    int w = 0;

    SDL_Texture *idleTexture = window->loadTexture("res/gfx/zombie/zombie_idle.png");
    spriteArray[ZOMBIE_IDLE_SPRITE] = {true, false, idleTexture,
                                       9 * w, 0,
                                       60, 0};

    SDL_Texture *walkTexture = window->loadTexture("res/gfx/zombie/zombie_walk.png");
    spriteArray[ZOMBIE_WALK_SPRITE] = {false, false, walkTexture,
                                       10 * w, 0,
                                       60, 0};

    SDL_Texture *runTexture = window->loadTexture("res/gfx/zombie/zombie_run.png");
    spriteArray[ZOMBIE_RUN_SPRITE] = {false, false, runTexture,
                                      8 * w, 0,
                                      90, 0};

    SDL_Texture *attackTexture = window->loadTexture("res/gfx/zombie/zombie_attack.png");
    spriteArray[ZOMBIE_ATTACK_SPRITE] = {false, true, attackTexture,
                                         4 * w, 0,
                                         60, 0};

    SDL_Texture *hurtTexture = window->loadTexture("res/gfx/zombie/zombie_hurt.png");
    spriteArray[ZOMBIE_HURT_SPRITE] = {false, true, hurtTexture,
                                       5 * w, 0,
                                       60, 0};

    texture = idleTexture;
}

void Monster::move(int dt) {
    Entity::move(dt);

//    if (x + renderW > SCREEN_WIDTH && xDirection == 1) {
    if (x > SCREEN_WIDTH && xVelocity > 0) {
        xVelocity = -MONSTER_WALK_SPEED;
    } else if (x < 0 && xVelocity < 0) {
        xVelocity = MONSTER_WALK_SPEED;
    }
}

void Monster::walk() {
    setSpriteAnimated(ZOMBIE_WALK_SPRITE, true);
    xVelocity = MONSTER_WALK_SPEED;
}

void Monster::run() {
    setSpriteAnimated(ZOMBIE_RUN_SPRITE, true);
    xVelocity = MONSTER_RUN_SPEED;
}

void Monster::attack() {
    setSpriteAnimated(ZOMBIE_ATTACK_SPRITE, true);
}

Monster *Monster::copy(WindowRenderer *window) {
    auto *copyMonster = new Monster(window);
    copyMonster->x = x;
    copyMonster->y = y;
    copyMonster->xVelocity = xVelocity;
    copyMonster->yVelocity = yVelocity;
    copyMonster->xVelocity = xVelocity;
    copyMonster->frame = frame;
    copyMonster->collisionRect = collisionRect;
    copyMonster->renderWMultiplier = renderWMultiplier;
    copyMonster->renderHMultiplier = renderHMultiplier;
    copyMonster->hp = hp;

    return copyMonster;
}
