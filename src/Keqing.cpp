//
// Created by pyrowildx on 13/05/23.
//

#include "Keqing.hpp"
#include "Particle.hpp"

Keqing *Keqing::instance = nullptr;

Keqing::Keqing(WindowRenderer *window)
        : AnimatedEntity(true, KQ_END_SPRITE_ENUM) {
    speed = KQ_SPEED;
    hp = 1;
    jumpVelocity = KQ_BASE_JUMP_VELOCITY;
    attackEndTime = 0;

    SDL_Texture *idleTexture = window->loadTexture("res/gfx/keqing/idle.png");
    spriteArray[KQ_IDLE_SPRITE] = {true, false, idleTexture,
                                   0, 0,
                                   96, 96,
                                   18 * 96, 0,
                                   60, 10000};

    SDL_Texture *walkTexture = window->loadTexture("res/gfx/keqing/walk.png");
    spriteArray[KQ_WALK_SPRITE] = {false, false, walkTexture,
                                   -10, 0,
                                   96, 96,
                                   8 * 96, 0,
                                   60, 0};

    SDL_Texture *jumpTexture = window->loadTexture("res/gfx/keqing/jump.png");
    spriteArray[KQ_JUMP_SPRITE] = {false, true, jumpTexture,
                                   0, -34,
                                   96, 128,
                                   7 * 96, 0,
                                   200, 0};

    SDL_Texture *attackTexture = window->loadTexture("res/gfx/keqing/attack.png");
    spriteArray[KQ_ATTACK_SPRITE] = {false, true, attackTexture,
                                     -38, -32,
                                     192, 160,
                                     34 * 192, 0,
                                     60, 0};

    SDL_Texture *hurtTexture = window->loadTexture("res/gfx/keqing/hurt.png");
    spriteArray[KQ_HURT_SPRITE] = {false, true, hurtTexture,
                                   0, 0,
                                   96, 96,
                                   6 * 96, 0,
                                   60, 0};

    SDL_Texture *turnTexture = window->loadTexture("res/gfx/keqing/turn.png");
    spriteArray[KQ_TURN_SPRITE] = {false, true, turnTexture,
                                   0, 0,
                                   96, 96,
                                   3 * 96, 0,
                                   120, 0};

    texture = idleTexture;
}

void Keqing::initKeqing(WindowRenderer *window) {
    instance = new Keqing(window);
}

void Keqing::updateDirection(int key, const bool *keyPressed) {
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
    setTextureAnimated(KQ_WALK_SPRITE, true);
}

void Keqing::clearDirection(int key, const bool *keyPressed) {
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
    setTextureAnimated(KQ_WALK_SPRITE, xDirection != 0 || zDirection != 0);
}

void Keqing::move(int dt) {
    Entity::move(dt);

    int minX = MIN_X;
    int maxX = MAX_X; // - renderW;
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

void Keqing::jump(int dt) {
    y -= (int) (jumpVelocity * (float) dt);

    jumpVelocity -= (float) dt * 0.0016f;

    if (y > DEFAULT_Y) {
        y = DEFAULT_Y;
        jumpVelocity = KQ_BASE_JUMP_VELOCITY;
        setTextureAnimated(KQ_JUMP_SPRITE, false);
    }
}

void Keqing::attack(int dt, int currentTime) {
    SpriteTexture *attackSprite = &spriteArray[KQ_ATTACK_SPRITE];
    const int numberOfAttacks = 5;
    int widthArray[numberOfAttacks] = {0,
                                       5 * attackSprite->width,
                                       11 * attackSprite->width,
                                       19 * attackSprite->width,
                                       27 * attackSprite->width};
    int dashLengthArray[numberOfAttacks] = {1, 0, 2, 0, 4};
    if (currentTime - attackEndTime < 2000) {
        int i;
        for (i = 1; i < numberOfAttacks; i++) {
            if (attackSprite->currentFrameX == widthArray[i]) {
                setTextureAnimated(KQ_ATTACK_SPRITE, false, false);
                attackEndTime = currentTime;
                attackSprite->currentFrameX += attackSprite->width;
                return;
            }
            if (attackSprite->currentFrameX < widthArray[i]) break;
        }
        i--;
        if (attackSprite->accumulatedTime += dt > attackSprite->timeBetweenFrames) {
            x += dashLengthArray[i];
        }
        if (i == 4) // TODO ONLY DO ONE
            Particle::push(PARTICLE_KQ_ATTACK_5, 0, 20,
                           4.0f, 4.0f, this);
    } else {
        setTextureAnimated(KQ_ATTACK_SPRITE, false);
        setTextureAnimated(KQ_ATTACK_SPRITE, true);
        attackEndTime = currentTime;
        return;
    }
}

void Keqing::damage(int dt) {
    if (!isDamaged()) {
        setTextureAnimated(KQ_HURT_SPRITE, true);
        x -= 10;
        hp--;
    } else {
        x -= (int) (KQ_KNOCKBACK_SDEED * (float) dt);
    }
}

void Keqing::destroy() {
    AnimatedEntity::destroy();
    delete this;
}
