//
// Created by pyrowildx on 13/05/23.
//

#include "Player.hpp"

Player::Player(int w, int h, WindowRenderer window)
        : AnimatedEntity(w, h, true, PLAYER_END_SPRITE_ENUM) {
    hp = 1;
    jumpVelocity = PLAYER_BASE_JUMP_VELOCITY;

    SDL_Texture *idleTexture = window.loadTexture("res/gfx/player/player_idle.png");
    spriteArray[PLAYER_IDLE_SPRITE] = {true, false, idleTexture,
                                       18 * w, 0,
                                       60, 0};

    SDL_Texture *runTexture = window.loadTexture("res/gfx/player/player_run.png");
    spriteArray[PLAYER_RUN_SPRITE] = {false, false, runTexture,
                                      24 * w, 0,
                                      60, 0};

    SDL_Texture *jumpTexture = window.loadTexture("res/gfx/player/player_jump.png");
    spriteArray[PLAYER_JUMP_SPRITE] = {false, true, jumpTexture,
                                       19 * w, 0,
                                       90, 0};

    SDL_Texture *attackTexture = window.loadTexture("res/gfx/player/player_attack.png");
    spriteArray[PLAYER_ATTACK_SPRITE] = {false, true, attackTexture,
                                         26 * w, 0,
                                         60, 0};

    SDL_Texture *hurtTexture = window.loadTexture("res/gfx/player/player_hurt.png");
    spriteArray[PLAYER_HURT_SPRITE] = {false, true, hurtTexture,
                                       7 * w, 0,
                                       60, 0};

    SDL_Texture *turnTexture = window.loadTexture("res/gfx/player/player_turn.png");
    spriteArray[PLAYER_TURN_SPRITE] = {false, true, turnTexture,
                                       5 * w, 0,
                                       120, 0};

    texture = idleTexture;
}

void Player::updateDirection(int key, const bool *keyPressed) {
    if (key == SDLK_q) {
        if (keyPressed[SDLK_d % 4]) xDirection = 0;
        else xDirection = -1;
    } else if (key == SDLK_d) {
        if (keyPressed[SDLK_q % 4]) xDirection = 0;
        else xDirection = 1;
    } else if (key == SDLK_z) {
        if (keyPressed[SDLK_s % 4]) zDirection = 0;
        else zDirection = -1;
    } else if (key == SDLK_s) {
        if (keyPressed[SDLK_z % 4]) zDirection = 0;
        else zDirection = 1;
    }
    setTextureAnimated(PLAYER_RUN_SPRITE, true);
}

void Player::clearDirection(int key, const bool *keyPressed) {
    if (key == SDLK_q) {
        if (keyPressed[SDLK_d % 4]) xDirection = 1;
        else xDirection = 0;
    } else if (key == SDLK_d) {
        if (keyPressed[SDLK_q % 4]) xDirection = -1;
        else xDirection = 0;
    } else if (key == SDLK_z) {
        if (keyPressed[SDLK_s % 4]) zDirection = 1;
        else zDirection = 0;
    } else if (key == SDLK_s) {
        if (keyPressed[SDLK_z % 4]) zDirection = -1;
        else zDirection = 0;
    }
    setTextureAnimated(PLAYER_RUN_SPRITE, xDirection != 0 || zDirection != 0);
}

void Player::move(int dt) {
    Entity::move(dt, PLAYER_SPEED);

    int minX = MIN_X;
    int maxX = MAX_X - renderW;
    if (x < minX) {
        x = minX;
    } else if (x > maxX) {
        x = maxX;
    }

    if (z < MIN_Z) {
        z = MIN_Z;
    } else if (z > MAX_Z) {
        z = MAX_Z;
    }
}

void Player::jump(int dt) {
    y -= (int) (jumpVelocity * (float) dt);

    jumpVelocity -= (float) dt * 0.0016f;

    if (y > PLAYER_DEFAULT_Y) {
        y = PLAYER_DEFAULT_Y;
        jumpVelocity = PLAYER_BASE_JUMP_VELOCITY;
        setTextureAnimated(PLAYER_JUMP_SPRITE, false);
    }
}

void Player::attack() {
    setTextureAnimated(PLAYER_ATTACK_SPRITE, true);
}

void Player::destroy() {
    AnimatedEntity::destroy();
}
