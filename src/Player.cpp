//
// Created by pyrowildx on 13/05/23.
//

#include "Player.hpp"

Player::Player(int x, int y, int z, int w, int h, WindowRenderer window)
        : Entity(x, y, z, w, h, nullptr) {
    hp = 1;
    SDL_Texture *idleTexture = window.loadTexture("res/gfx/player_idle.png");
    idleSprite = {true, idleTexture, 18 * w, 60};

    SDL_Texture *runTexture = window.loadTexture("res/gfx/player_run.png");
    runSprite = {false, runTexture, 24 * w, 60};

    SDL_Texture *jumpTexture = window.loadTexture("res/gfx/player_jump.png");
    jumpSprite = {false, jumpTexture, 19 * w, 60};

    SDL_Texture *attackTexture = window.loadTexture("res/gfx/player_attack.png");
    attackSprite = {false, attackTexture, 26 * w, 60};

    SDL_Texture *hurtTexture = window.loadTexture("res/gfx/player_hurt.png");
    hurtSprite = {false, hurtTexture, 7 * w, 60};

    currentSprite = idleSprite;
    texture = currentSprite.texture;
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
    runSprite.animated = true;
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
    runSprite.animated = xDirection != 0 || zDirection != 0;
}

void Player::jump() {
    y -= 60;
}

void Player::updateTexture() {
    if (idleSprite.animated) currentSprite = idleSprite;
    if (runSprite.animated) currentSprite = runSprite;
    if (jumpSprite.animated) currentSprite = jumpSprite;
    if (attackSprite.animated) currentSprite = attackSprite;
    if (hurtSprite.animated) currentSprite = hurtSprite;

    texture = currentSprite.texture;
}

void Player::animate(Uint32 *accumulatedAnimationTime) {
    if (*accumulatedAnimationTime > currentSprite.timeBetweenFrames) {
        frame.x += frame.w;
        if (frame.x >= currentSprite.totalWidth) {
            frame.x = 0;
        }
        *accumulatedAnimationTime = 0;
    }
}

void Player::destroy() {
    SDL_DestroyTexture(idleSprite.texture);
    SDL_DestroyTexture(runSprite.texture);
    SDL_DestroyTexture(jumpSprite.texture);
    SDL_DestroyTexture(attackSprite.texture);
    SDL_DestroyTexture(hurtSprite.texture);
}
