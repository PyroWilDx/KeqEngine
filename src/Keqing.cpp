//
// Created by pyrowildx on 13/05/23.
//

#include "Keqing.hpp"
#include "Particle.hpp"

#define MAX_TIME_BETWEEN_NATTACKS 400

#define NATTACKS_SPRITE_WIDTH 192
#define STARWARD_SWORD_SPRITE_WIDTH 128

Keqing *Keqing::instance = nullptr;

Keqing::Keqing(WindowRenderer *window)
        : AnimatedEntity(true, KQ_END_SPRITE_ENUM) {
    speed = KQ_SPEED;
    hp = 1;
    jumpVelocity = KQ_BASE_JUMP_VELOCITY;
    lastNAttackTime = 0;
    dashXToAdd = 0;

    spriteArray[KQ_IDLE_SPRITE] =
            {KQ_IDLE_SPRITE, true,
             window->loadTexture("res/gfx/keqing/idle.png"),
             0, 0, -36,
             96, 96,
             18 * 96, 60,
             0, 10000,
             &spriteArray[KQ_IDLE_SPRITE]};

    spriteArray[KQ_JUMP_LAND_SPRITE] =
            {KQ_JUMP_LAND_SPRITE, false,
             window->loadTexture("res/gfx/keqing/jump_land.png"),
             0, 0, -34,
             96, 96,
             2 * 96, 80,
             0, 0,
             nullptr};

    spriteArray[KQ_DASH_STOP_SPRITE] =
            {KQ_DASH_STOP_SPRITE, false,
             window->loadTexture("res/gfx/keqing/dash_stop.png"),
             0, 0, -36,
             96, 96,
             5 * 96, 90,
             0, 0,
             nullptr};

    spriteArray[KQ_WALK_SPRITE] =
            {KQ_WALK_SPRITE, false,
             window->loadTexture("res/gfx/keqing/walk.png"),
             -10, 0, -24,
             96, 96,
             8 * 96, 60,
             0, 0,
             &spriteArray[KQ_WALK_SPRITE]};

    spriteArray[KQ_TURN_SPRITE] =
            {KQ_TURN_SPRITE, false,
             window->loadTexture("res/gfx/keqing/turn.png"),
             -16, 0, -20,
             96, 96,
             3 * 96, 30,
             0, 0,
             nullptr};

    spriteArray[KQ_JUMP_START_SPRITE] =
            {KQ_JUMP_START_SPRITE, false,
             window->loadTexture("res/gfx/keqing/jump_start.png"),
             0, 0, -36,
             96, 96,
             2 * 96, 20,
             0, 0,
             &spriteArray[KQ_JUMP_SPRITE]};

    spriteArray[KQ_JUMP_SPRITE] =
            {KQ_JUMP_SPRITE, false,
             window->loadTexture("res/gfx/keqing/jump.png"),
             0, -34, -34,
             96, 128,
             7 * 96, 200,
             0, 0,
             &spriteArray[KQ_JUMP_LAND_SPRITE]};

    spriteArray[KQ_NATTACKS_SPRITE] =
            {KQ_NATTACKS_SPRITE, false,
             window->loadTexture("res/gfx/keqing/nattacks.png"),
             -38, -32, -92,
             NATTACKS_SPRITE_WIDTH, 160,
             34 * NATTACKS_SPRITE_WIDTH, 60,
             0, 0,
             nullptr};

    spriteArray[KQ_DASH_START_SPRITE] =
            {KQ_DASH_START_SPRITE, false,
             window->loadTexture("res/gfx/keqing/dash_start.png"),
             -4, 0, -32,
             96, 96,
             3 * 96, 20,
             0, 0,
             &spriteArray[KQ_DASH_SPRITE]};

    spriteArray[KQ_DASH_SPRITE] =
            {KQ_DASH_SPRITE, false,
             window->loadTexture("res/gfx/keqing/dash.png"),
             -32, 0, -36,
             128, 96,
             8 * 128, 20,
             0, 0,
             &spriteArray[KQ_DASH_STOP_SPRITE]};

    spriteArray[KQ_TURN_DASH_SPRITE] =
            {KQ_TURN_DASH_SPRITE, false,
             window->loadTexture("res/gfx/keqing/turn_dash.png"),
             -20, 0, -16,
             96, 96,
             3 * 96, 40,
             0, 0,
             nullptr};

    spriteArray[KQ_JUMP_DASH_SPRITE] =
            {KQ_JUMP_DASH_SPRITE, false,
             window->loadTexture("res/gfx/keqing/jump_dash.png"),
             -10, 0, -26,
             96, 96,
             6 * 96, 40,
             0, 0,
             nullptr};

    spriteArray[KQ_AIR_NATTACK_SPRITE] =
            {KQ_AIR_NATTACK_SPRITE, false,
             window->loadTexture("res/gfx/keqing/air_nattack.png"),
             -30, -32, -36,
             128, 128,
             14 * 128, 60,
             0, 0,
             nullptr};

    spriteArray[KQ_HURT_SPRITE] =
            {KQ_HURT_SPRITE, false,
             window->loadTexture("res/gfx/keqing/hurt.png"),
             0, 0, 0,
             96, 96,
             6 * 96, 60,
             0, 0,
             nullptr};

    spriteArray[KQ_STARWARD_SWORD_SPRITE] =
            {KQ_STARWARD_SWORD_SPRITE, false,
             window->loadTexture("res/gfx/keqing/starward_sword.png"),
             -24, 0, -44,
             128, 96,
             30 * 128, 70,
             0, 0,
             nullptr};

    texture = spriteArray[KQ_IDLE_SPRITE].texture;
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
    Sprite *turnSprite = &spriteArray[KQ_TURN_SPRITE];
    if (keyPressed[SDLK_q % 4] && keyPressed[SDLK_d % 4]) {
        forceSprite(KQ_TURN_SPRITE, 2 * turnSprite->width,
                    INT32_MAX, 1 * turnSprite->width);
    } else {
        removeForcedSprite();
    }
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
    removeForcedSprite();
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
        if (spriteArray[KQ_JUMP_SPRITE].next != nullptr) {
            spriteArray[KQ_JUMP_SPRITE].next->animated = true;
        }
    }
}

const int numberOfNAttacks = 5;
const int nattacksWidths[numberOfNAttacks] = {0,
                                              4 * NATTACKS_SPRITE_WIDTH,
                                              11 * NATTACKS_SPRITE_WIDTH,
                                              18 * NATTACKS_SPRITE_WIDTH,
                                              27 * NATTACKS_SPRITE_WIDTH};
const int nattacksDashesX[numberOfNAttacks] = {60, 0, 100,
                                               0, 400};

void Keqing::nattack(int dt, int currentTime) {
    int lastTime = lastNAttackTime;
    lastNAttackTime = currentTime;
    Sprite *nattacksSprite = &spriteArray[KQ_NATTACKS_SPRITE];
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
        dashXToAdd += (float) (nattacksDashesX[i] * dt) * 0.001f;
        int rounded = (int) dashXToAdd;
        dashXToAdd -= (float) rounded;
        if (!facingEast) rounded = -rounded;
        x += rounded;

        if (isNewestFrame(nattacksSprite,
                          nattacksWidths[4] + nattacksSprite->width)) {
            Particle::push(PARTICLE_KQ_NATTACK_4,
                           -6, 20, -40, 60,
                           2.0f, 2.0f, this);
        }
    } else {
        nattacksSprite->currentFrameX = 0;
        nattacksSprite->accumulatedTime = 0;
        return;
    }
}

void Keqing::airNAttack(int dt) {
    // TODO
    Sprite *airNAttackSprite = &spriteArray[KQ_AIR_NATTACK_SPRITE];
    if (isNewestFrame(airNAttackSprite, 0)) {
        Particle::push(PARTICLE_KQ_AIR_NATTACK,
                       12, -26, -14, 200,
                       2.0f, 2.0f, this);
    }
}

void Keqing::stellarRestoration() {
    // TODO
}

void Keqing::starwardSword() {
    // TODO
    Sprite *ssSprite = &spriteArray[KQ_STARWARD_SWORD_SPRITE];
    if (isNewestFrame(ssSprite, 0)) {
        int pXShift = -86;
        int pYShift = -74;
        int pXShiftR = -86;
        Particle::push(PARTICLE_KQ_SS_AOE,
                       pXShift, pYShift, pXShiftR, 300,
                       2.0f, 2.0f, this);
        Particle::push(PARTICLE_KQ_SS_AOE_WAVES,
                       pXShift, pYShift, pXShiftR, 175,
                       2.0f, 2.0f, this);
        Particle::push(PARTICLE_KQ_SS_CLONES,
                       pXShift, pYShift, pXShiftR, 91,
                       2.0f, 2.0f, this);
    } else if (isNewestFrame(ssSprite,
                             28 * STARWARD_SWORD_SPRITE_WIDTH)) {
        Particle::push(PARTICLE_KQ_SS_FINAL_SLASH,
                       -356, -112, -356, 60,
                       1.0f, 1.0f, this);
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

void Keqing::jumpDash(int dt) {
    int toAdd = (int) (KQ_JUMP_DASH_SPEED * (float) dt);
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
            spriteArray[KQ_DASH_SPRITE].animated) {
            setTextureAnimated(KQ_TURN_DASH_SPRITE, true);
        } else {
            setTextureAnimated(KQ_TURN_SPRITE, true);
        }
        facingEast = value;
    }
}

void Keqing::destroy() {
    AnimatedEntity::destroy();
    delete this;
}
