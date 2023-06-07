//
// Created by pyrowildx on 13/05/23.
//

#include "Keqing.hpp"
#include "Particle.hpp"

#define MAX_TIME_BETWEEN_NATTACKS 1000
#define NATTACKS_SPRITE_WIDTH 192

Keqing *Keqing::instance = nullptr;

Keqing::Keqing(WindowRenderer *window)
        : AnimatedEntity(true, KQ_END_SPRITE_ENUM) {
    speed = KQ_SPEED;
    hp = 1;
    jumpVelocity = KQ_BASE_JUMP_VELOCITY;
    lastNAttackTime = 0;
    dashXToAdd = 0;

    SDL_Texture *idleTexture = window->loadTexture("res/gfx/keqing/idle.png");
    spriteArray[KQ_IDLE_SPRITE] = {true, false, idleTexture,
                                   0, 0, -36,
                                   96, 96,
                                   18 * 96, 60,
                                   0, 10000,
                                   nullptr};

    SDL_Texture *dashStopTexture = window->loadTexture("res/gfx/keqing/dash_stop.png");
    spriteArray[KQ_DASH_STOP_SPRITE] = {false, true, dashStopTexture,
                                        0, 0, -36,
                                        96, 96,
                                        5 * 96, 90,
                                        0, 0,
                                        nullptr};

    SDL_Texture *walkTexture = window->loadTexture("res/gfx/keqing/walk.png");
    spriteArray[KQ_WALK_SPRITE] = {false, false, walkTexture,
                                   -10, 0, -24,
                                   96, 96,
                                   8 * 96, 60,
                                   0, 0,
                                   nullptr};

    SDL_Texture *turnTexture = window->loadTexture("res/gfx/keqing/turn.png");
    spriteArray[KQ_TURN_SPRITE] = {false, true, turnTexture,
                                   -16, 0, -20,
                                   96, 96,
                                   3 * 96, 30,
                                   0, 0,
                                   nullptr};

    SDL_Texture *jumpTexture = window->loadTexture("res/gfx/keqing/jump.png");
    spriteArray[KQ_JUMP_SPRITE] = {false, true, jumpTexture,
                                   0, -34, -34,
                                   96, 128,
                                   7 * 96, 200,
                                   0, 0,
                                   nullptr};

    SDL_Texture *nattacksTexture = window->loadTexture("res/gfx/keqing/nattacks.png");
    spriteArray[KQ_NATTACKS_SPRITE] = {false, true, nattacksTexture,
                                       -38, -32, -92,
                                       NATTACKS_SPRITE_WIDTH, 160,
                                       34 * NATTACKS_SPRITE_WIDTH, 60,
                                       0, 0,
                                       nullptr};

    SDL_Texture *dashStartTexture = window->loadTexture("res/gfx/keqing/dash_start.png");
    spriteArray[KQ_DASH_START_SPRITE] = {false, true, dashStartTexture,
                                         -4, 0, -32,
                                         96, 96,
                                         3 * 96, 20,
                                         0, 0,
                                         nullptr};

    SDL_Texture *dashTexture = window->loadTexture("res/gfx/keqing/dash.png");
    spriteArray[KQ_DASH_SPRITE] = {false, true, dashTexture,
                                   -32, 0, -36,
                                   128, 96,
                                   8 * 128, 20,
                                   0, 0,
                                   nullptr};

    SDL_Texture *turnDashTexture = window->loadTexture("res/gfx/keqing/turn_dash.png");
    spriteArray[KQ_TURN_DASH_SPRITE] = {false, true, turnDashTexture,
                                        -20, 0, -16,
                                        96, 96,
                                        3 * 96, 40,
                                        0, 0,
                                        nullptr};

    SDL_Texture *hurtTexture = window->loadTexture("res/gfx/keqing/hurt.png");
    spriteArray[KQ_HURT_SPRITE] = {false, true, hurtTexture,
                                   0, 0, 0,
                                   96, 96,
                                   6 * 96, 60,
                                   0, 0,
                                   nullptr};

    spriteArray[KQ_DASH_START_SPRITE].next = &spriteArray[KQ_DASH_SPRITE];
    spriteArray[KQ_DASH_SPRITE].next = &spriteArray[KQ_DASH_STOP_SPRITE];

    texture = idleTexture;
}

void Keqing::initKeqing(WindowRenderer *window) {
    instance = new Keqing(window);
}

void Keqing::updateDirection(int key, const bool *keyPressed) {
    if (key == SDLK_q) {
        if (keyPressed[SDLK_d % 4]) xDirection = 0;
        else xDirection = -1;
        setFacingEast(false);
    } else if (key == SDLK_d) {
        if (keyPressed[SDLK_q % 4]) xDirection = 0;
        else xDirection = 1;
        setFacingEast(true);
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
        if (keyPressed[SDLK_d % 4]) {
            xDirection = 1;
            setFacingEast(true);
        } else {
            xDirection = 0;
        }
    } else if (key == SDLK_d) {
        if (keyPressed[SDLK_q % 4]) {
            xDirection = -1;
            setFacingEast(false);
        } else {
            xDirection = 0;
        }
    } else if (key == SDLK_z) {
        if (keyPressed[SDLK_s % 4]) zDirection = 1;
        else zDirection = 0;
    } else if (key == SDLK_s) {
        if (keyPressed[SDLK_z % 4]) zDirection = -1;
        else zDirection = 0;
    }
    if (xDirection == 0 && zDirection == 0) {
        setTextureAnimated(KQ_WALK_SPRITE, false);
    }
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

const int numberOfNAttacks = 5;
const int nattacksWidths[numberOfNAttacks] = {0,
                                              4 * NATTACKS_SPRITE_WIDTH,
                                              11 * NATTACKS_SPRITE_WIDTH,
                                              18 * NATTACKS_SPRITE_WIDTH,
                                              27 * NATTACKS_SPRITE_WIDTH};
const int nattacksDashsX[numberOfNAttacks] = {60, 0, 100,
                                              0, 400};

void Keqing::nattack(int dt, int currentTime) {
    int lastTime = lastNAttackTime;
    lastNAttackTime = currentTime;
    SpriteTexture *nattacksSprite = &spriteArray[KQ_NATTACKS_SPRITE];
    if (currentTime - lastTime < MAX_TIME_BETWEEN_NATTACKS) {
        int i;
        for (i = 1; i < numberOfNAttacks; i++) {
            if (nattacksSprite->currentFrameX == nattacksWidths[i]) {
                setTextureAnimated(KQ_NATTACKS_SPRITE, false, false);
                nattacksSprite->currentFrameX += nattacksSprite->width;
                return;
            }
            if (nattacksSprite->currentFrameX < nattacksWidths[i]) break;
        }
        i--;
        dashXToAdd += (float) (nattacksDashsX[i] * dt) * 0.001f;
        int rounded = (int) dashXToAdd;
        dashXToAdd -= (float) rounded;
        if (!facingEast) rounded = -rounded;
        x += rounded;

        if (nattacksSprite->currentFrameX == nattacksWidths[4] + nattacksSprite->width &&
            nattacksSprite->accumulatedTime == 0) {
            Particle::push(PARTICLE_KQ_NATTACK_4,
                           -6, 20, -40,
                           2.0f * KQ_WIDTH_MULTIPLIER,
                           2.0f * KQ_HEIGHT_MULTIPLIER,
                           this);
        }
    } else {
        nattacksSprite->currentFrameX = 0;
        nattacksSprite->accumulatedTime = 0;
        return;
    }
}

void Keqing::dash(int dt) {
    float coeff = 1.0f;
    if (spriteArray[KQ_DASH_START_SPRITE].animated) coeff = 0.4f;
    else if (spriteArray[KQ_DASH_STOP_SPRITE].animated) coeff = 0.2f;

    int toAdd = (int) (KQ_DASH_SPEED * (float) dt * coeff);
    if (!facingEast) toAdd = -toAdd;
    x += toAdd;
}

void Keqing::damage(int dt) { // TODO change
    if (!isDamaged()) {
        setTextureAnimated(KQ_HURT_SPRITE, true);
        x -= 10;
        hp--;
    } else {
        x -= (int) (KQ_KNOCKBACK_SPEED * (float) dt);
    }
}

void Keqing::setFacingEast(bool value) {
    if (facingEast != value) {
        if (spriteArray[KQ_DASH_START_SPRITE].animated ||
            spriteArray[KQ_DASH_SPRITE].animated)
            setTextureAnimated(KQ_TURN_DASH_SPRITE, true);
        else
            setTextureAnimated(KQ_TURN_SPRITE, true);
    }
    facingEast = value;
}

void Keqing::destroy() {
    AnimatedEntity::destroy();
    delete this;
}
