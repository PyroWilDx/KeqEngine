//
// Created by pyrowildx on 30/05/23.
//

#include "Monster.hpp"

Monster::Monster(int w, int h, WindowRenderer *window)
        : AnimatedEntity(w, h, true, ZOMBIE_END_SPRITE_ENUM) {
    xDirection = 1;
    speed = MONSTER_WALK_SPEED;
    hp = 1;

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

    if (x + renderW > SCREEN_WIDTH && xDirection == 1) {
        xDirection = -1;
    } else if (x < 0 && xDirection == -1) {
        xDirection = 1;
    }
}

void Monster::walk() {
    setTextureAnimated(ZOMBIE_WALK_SPRITE, true);
    speed = MONSTER_WALK_SPEED;
}

void Monster::run() {
    setTextureAnimated(ZOMBIE_RUN_SPRITE, true);
    speed = MONSTER_RUN_SPEED;
}

void Monster::attack() {
    setTextureAnimated(ZOMBIE_ATTACK_SPRITE, true);
}

Monster *Monster::copy(WindowRenderer *window) {
    auto *copyMonster = new Monster(frame.w, frame.h, window);
    copyMonster->x = x;
    copyMonster->y = y;
    copyMonster->z = z;
    copyMonster->xDirection = xDirection;
    copyMonster->yDirection = yDirection;
    copyMonster->zDirection = zDirection;
    copyMonster->speed = speed;
    copyMonster->frame = frame;
    copyMonster->collisionRect = collisionRect;
    copyMonster->hasShadow = hasShadow;
    copyMonster->renderW = renderW;
    copyMonster->renderH = renderH;
    copyMonster->hp = hp;

    return copyMonster;
}

void Monster::destroy() {
    AnimatedEntity::destroy();
}
