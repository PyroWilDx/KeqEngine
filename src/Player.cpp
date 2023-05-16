//
// Created by pyrowildx on 13/05/23.
//

#include "Player.hpp"

Player::Player(int x, int y, int z, int w, int h, WindowRenderer window)
        : Entity(x, y, z, w, h, nullptr) {
    hp = 1;
    jumpVelocity = 0;

    SDL_Texture *idleTexture = window.loadTexture("res/gfx/player_idle.png");
    spriteArray[IDLE_SPRITE] = {true, idleTexture, 18 * w, 60};

    SDL_Texture *runTexture = window.loadTexture("res/gfx/player_run.png");
    spriteArray[RUN_SPRITE] = {false, runTexture, 24 * w, 60};

    SDL_Texture *jumpTexture = window.loadTexture("res/gfx/player_jump.png");
    spriteArray[JUMP_SPRITE] = {false, jumpTexture, 19 * w, 80};

    SDL_Texture *attackTexture = window.loadTexture("res/gfx/player_attack.png");
    spriteArray[ATTACK_SPRITE] = {false, attackTexture, 26 * w, 60};

    SDL_Texture *hurtTexture = window.loadTexture("res/gfx/player_hurt.png");
    spriteArray[HURT_SPRITE] = {false, hurtTexture, 7 * w, 60};

    SDL_Texture *turnTexture = window.loadTexture("res/gfx/player_turn.png");
    spriteArray[TURN_SPRITE] = {false, turnTexture, 5 * w, 120};

    currentSprite = IDLE_SPRITE;
    texture = idleTexture;
}

void Player::setTextureAnimated(int code, bool animated) {
    spriteArray[code].animated = animated;
}

void Player::updateTexture() {
    for (int i = 0; i < END_SPRITE_ENUM; i++) {
        if (spriteArray[i].animated) {
            currentSprite = i;
        }
    }

    if (texture != spriteArray[currentSprite].texture) {
        texture = spriteArray[currentSprite].texture;
        frame.x = 0;
    }
}

void Player::updateDirection(int key, const bool *keyPressed) {
    if (key == SDLK_q) {
        if (keyPressed[SDLK_d % 4]) xDirection = 0;
        else xDirection = -1;
    } else if (key == SDLK_d) {
        if (keyPressed[SDLK_q % 4]) xDirection = 0;
        else xDirection = 1;
    } else if (key == SDLK_s) {
        if (keyPressed[SDLK_z % 4]) zDirection = 0;
        else zDirection = -1;
    } else if (key == SDLK_z) {
        if (keyPressed[SDLK_s % 4]) zDirection = 0;
        else zDirection = 1;
    }
    spriteArray[RUN_SPRITE].animated = true;
}

void Player::clearDirection(int key, const bool *keyPressed) {
    if (key == SDLK_q) {
        if (keyPressed[SDLK_d % 4]) xDirection = 1;
        else xDirection = 0;
    } else if (key == SDLK_d) {
        if (keyPressed[SDLK_q % 4]) xDirection = -1;
        else xDirection = 0;
    } else if (key == SDLK_s) {
        if (keyPressed[SDLK_z % 4]) zDirection = 1;
        else zDirection = 0;
    } else if (key == SDLK_z) {
        if (keyPressed[SDLK_s % 4]) zDirection = -1;
        else zDirection = 0;
    }
    spriteArray[RUN_SPRITE].animated = xDirection != 0 || zDirection != 0;
}

void Player::move(int dt) {
    int tmp = (int) ((float) dt * PLAYER_SPEED);
    x += xDirection * tmp;
    z += zDirection * tmp;
}

void Player::jump(int dt) {
    y -= (int) (jumpVelocity * (float) dt);
    if (jumpVelocity >= 0) {
        if (y > PLAYER_MIN_Y) {
            jumpVelocity += (float) dt * 0.001f;
        } else if (y <= PLAYER_MIN_Y) {
            y = PLAYER_MIN_Y;
            jumpVelocity = -0.2f;
        }
    }
    if (y > PLAYER_DEFAULT_Y) {
        y = PLAYER_DEFAULT_Y;
        jumpVelocity = 0;
        setTextureAnimated(JUMP_SPRITE, false);
    }
}

void Player::animate(Uint32 *accumulatedAnimationTime) {
    if (*accumulatedAnimationTime > spriteArray[currentSprite].timeBetweenFrames) {
        frame.x += frame.w;
        if (frame.x >= spriteArray[currentSprite].totalWidth) {
            frame.x = 0;
        }
        *accumulatedAnimationTime = 0;
    }
}

void Player::destroy() {
    for (int i = 0; i < END_SPRITE_ENUM; i++) {
        SDL_DestroyTexture(spriteArray[i].texture);
    }
}
