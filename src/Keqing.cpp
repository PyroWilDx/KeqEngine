//
// Created by pyrowildx on 13/05/23.
//

#include "Keqing.hpp"
#include "Particle.hpp"

#define MAX_TIME_BETWEEN_NATTACKS 400

#define NATTACKS_SPRITE_WIDTH 192

#define STARWARD_SWORD_FRAME_DURATION 70

Keqing *Keqing::instance = nullptr;

Keqing::Keqing(WindowRenderer *window)
        : AnimatedEntity(true, KQ_END_SPRITE_ENUM) {
    hp = 1;
    yVelocity = KQ_BASE_JUMP_VELOCITY;
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
             3 * 96, 20,
             0, 0,
             nullptr};

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

    spriteArray[KQ_STARWARD_SWORD_SPRITE] =
            {KQ_STARWARD_SWORD_SPRITE, false,
             window->loadTexture("res/gfx/keqing/starward_sword.png"),
             -24, 0, -44,
             128, 96,
             30 * 128, STARWARD_SWORD_FRAME_DURATION,
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

    spriteArray[KQ_AIR_DASH_SPRITE] =
            {KQ_AIR_DASH_SPRITE, false,
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

    texture = spriteArray[KQ_IDLE_SPRITE].texture;
}

void Keqing::initKeqing(WindowRenderer *window) {
    instance = new Keqing(window);
}

void Keqing::updateDirection(const bool *pressedKeys, int lastKey) {
    if (pressedKeys[SDLK_q % 4]) {
        if (pressedKeys[SDLK_d % 4]) {
            xVelocity = 0;
            if (lastKey == SDLK_q) setFacingEast(false);
            else if (lastKey == SDLK_d) setFacingEast(true);
        } else {
            xVelocity = -KQ_WALK_SPEED;
            setFacingEast(false);
        }
    } else if (pressedKeys[SDLK_d % 4]) {
        xVelocity = KQ_WALK_SPEED;
        setFacingEast(true);
    } else {
        xVelocity = 0;
    }
    if (pressedKeys[SDLK_z % 4]) {
        if (pressedKeys[SDLK_s % 4]) zVelocity = 0;
        else zVelocity = -Z_VELOCITY;
    } else if (pressedKeys[SDLK_s % 4]) {
        zVelocity = Z_VELOCITY;
    } else {
        zVelocity = 0;
    }

    if (xVelocity != 0 || zVelocity != 0) {
        setSpriteAnimated(KQ_WALK_SPRITE, true);
    } else {
        setSpriteAnimated(KQ_WALK_SPRITE, false);
    }

    Sprite *turnSprite = &spriteArray[KQ_TURN_SPRITE];
    if (pressedKeys[SDLK_q % 4] && pressedKeys[SDLK_d % 4]) {
        forceSprite(KQ_TURN_SPRITE, 2 * turnSprite->width,
                    INT32_MAX, 1 * turnSprite->width);
    } else {
        removeForcedSprite();
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
                setSpriteAnimated(KQ_NATTACKS_SPRITE, false, false);
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

void Keqing::dash() {
    float coeff;
    if (spriteArray[KQ_DASH_STOP_SPRITE].animated) coeff = 0.2f;
    if (spriteArray[KQ_DASH_START_SPRITE].animated) coeff = 0.4f;
    if (spriteArray[KQ_DASH_SPRITE].animated) coeff = 1.0f;

    float lastXVelocity = xVelocity;
    xVelocity = KQ_DASH_SPEED * coeff;
    if (!facingEast) {
        xVelocity = -xVelocity;
        if (xVelocity > lastXVelocity) xVelocity = lastXVelocity;
    } else {
        if (xVelocity < lastXVelocity) xVelocity = lastXVelocity;
    }
}


void Keqing::stellarRestoration() {
    // TODO
}

const int cSlashFrameDuration = 20;

const int cSlashRotations[KQ_SS_NUMBER_OF_CLONE_SLASH] =
        {10, 200, 354, 152, 306, 90};

const int cSlashXShift[KQ_SS_NUMBER_OF_CLONE_SLASH] =
        {0, -86, -80, -110, -132, -70};

const int cSlashYShift[KQ_SS_NUMBER_OF_CLONE_SLASH] =
        {34, 6, -46, 12, -36, 0};

const int cSlashXShiftR[KQ_SS_NUMBER_OF_CLONE_SLASH] =
        {0, 0, 0, 0, 0, 0};

const float cSlashWM[KQ_SS_NUMBER_OF_CLONE_SLASH] =
        {0.88f, 1.6f, 1.2f, 1.52f, 1.32f, 1.0f};

const float cSlashHM[KQ_SS_NUMBER_OF_CLONE_SLASH] =
        {1.0f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f};

static void pushCloneSlashParticle(Entity *keqing) {
    Particle *cSlashParticle =
            Particle::push(PARTICLE_KQ_SS_CLONE_SLASH,
                           cSlashXShift[0],
                           cSlashYShift[0],
                           cSlashXShiftR[0],
                           cSlashFrameDuration,
                           cSlashWM[0],
                           cSlashHM[0],
                           keqing);
    cSlashParticle->setRotation(cSlashRotations[0]);

    Particle *lastParticle = cSlashParticle;
    for (int i = 1; i < KQ_SS_NUMBER_OF_CLONE_SLASH; i++) {
        auto *nextCSlashParticle = new Particle(PARTICLE_KQ_SS_CLONE_SLASH,
                                                cSlashXShift[i],
                                                cSlashYShift[i],
                                                cSlashXShiftR[i],
                                                cSlashFrameDuration,
                                                cSlashWM[i],
                                                cSlashHM[i],
                                                keqing);
        nextCSlashParticle->setRotation(cSlashRotations[i]);
        lastParticle->setNextParticle(nextCSlashParticle);
        lastParticle = nextCSlashParticle;
    }
}

const int slashDuration = 20;
const int slashRotations[KQ_SS_NUMBER_OF_SLASH] =
        {0, 20, 40, 60, 80, 100, 120, 140};

static void pushSlashParticle(Entity *keqing) {
    for (int i = 0; i < KQ_SS_NUMBER_OF_SLASH; i++) {
        Particle *slashParticle =
                Particle::push(PARTICLE_KQ_SS_SLASH,
                               -196, 30, 0,
                               slashDuration,
                               1.2f, 1.2f, keqing);
        slashParticle->setRotation(slashRotations[i]);
        slashParticle->delay(0, 3 * slashDuration * i);
    }
}

void Keqing::starwardSword() {
    Sprite *ssSprite = &spriteArray[KQ_STARWARD_SWORD_SPRITE];
    if (isNewestFrame(ssSprite, 0)) { // Burst Start
        int pXShift = -84;
        int pYShift = -34;
        int pXShiftR = -84;
        Particle *aoeParticle =
                Particle::push(PARTICLE_KQ_SS_AOE,
                               pXShift, pYShift, pXShiftR,
                               80, 2.0f, 2.0f,
                               this);
        aoeParticle->setNextParticle(aoeParticle);
        Particle *aoeWaveParticle =
                Particle::push(PARTICLE_KQ_SS_AOE_WAVE,
                               pXShift, pYShift, pXShiftR,
                               60, 1.0f, 1.0f,
                               this);
        aoeWaveParticle->setRGBAMod(255, 255, 255, 128);
        aoeWaveParticle->setNextParticle(aoeWaveParticle);

    } else if (isNewestFrame(ssSprite, 12 * ssSprite->width)) { // Vanish
        Particle::push(PARTICLE_KQ_SS_VANISH,
                       -22, 6, -14,
                       60, 1.0f, 1.0f,
                       this);

    } else if (isNewestFrame(ssSprite, 13 * ssSprite->width)) { // Clone Slash Start
        pushCloneSlashParticle(this);
        Particle *cloneParticle =
                Particle::push(PARTICLE_KQ_SS_CLONE,
                               -86, -66, -86,
                               cSlashFrameDuration * 1.4f,
                               2.0f, 2.0f, this);
        cloneParticle->delay(0, cSlashFrameDuration);
        cloneParticle->setStopOnLastFrame(true);
        ssSprite->frameDuration = INT32_MAX;

    } else if (isNewestFrame(ssSprite, 28 * ssSprite->width)) { // Final Slash
        Particle *tmpParticle = Particle::getParticle(PARTICLE_KQ_SS_AOE, 0);
        tmpParticle->fadeAway();
        tmpParticle = Particle::getParticle(PARTICLE_KQ_SS_AOE_WAVE, 0);
        tmpParticle->fadeAway();
        Particle::remove(PARTICLE_KQ_SS_CLONE, 0);
        Particle::push(PARTICLE_KQ_SS_FINAL_SLASH,
                       -356, -112, -356,
                       60, 1.0f, 1.0f,
                       this);
    }

    if (ssSprite->currentFrameX == 13 * ssSprite->width &&
        !Particle::isActive(PARTICLE_KQ_SS_CLONE_SLASH, 0)) { // Clone Slash End
        ssSprite->frameDuration = STARWARD_SWORD_FRAME_DURATION;
        ssSprite->currentFrameX += ssSprite->width;
        pushSlashParticle(this);
    }

}

void Keqing::jump(int dt) {
    y -= (int) (yVelocity * (float) dt);

    yVelocity -= (float) dt * 0.0016f;

    if (y > DEFAULT_Y) {
        y = DEFAULT_Y;
        yVelocity = KQ_BASE_JUMP_VELOCITY;
        setSpriteAnimated(KQ_JUMP_SPRITE, false);
        if (spriteArray[KQ_JUMP_SPRITE].next != nullptr) {
            spriteArray[KQ_JUMP_SPRITE].next->animated = true;
        }
    }
}

void Keqing::airNAttack(int dt) { // Plunge Attack in Genshin
    Sprite *airNAttackSprite = &spriteArray[KQ_AIR_NATTACK_SPRITE];

    if (isNewestFrame(airNAttackSprite, 0)) {
        setSpriteAnimated(KQ_JUMP_SPRITE, false);
        yVelocity = 0.0f;
    } else if (isNewestFrame(airNAttackSprite, 1 * airNAttackSprite->width)) {
        yVelocity = 0.4f;
    } else if (isNewestFrame(airNAttackSprite, 6 * airNAttackSprite->width)) {
        Particle::push(PARTICLE_KQ_AIR_NATTACK, // TODO
                       8, -28, -12, 100,
                       2.2f, 2.0f, this);
    }

    if (airNAttackSprite->currentFrameX < 9 * airNAttackSprite->width) {
        y -= (int) (yVelocity * (float) dt);

        yVelocity -= (float) dt * 0.002f;

        if (y > DEFAULT_Y) {
            y = DEFAULT_Y;
            yVelocity = KQ_BASE_JUMP_VELOCITY;
            moveSpriteFrameX(KQ_AIR_NATTACK_SPRITE, 9 * airNAttackSprite->width);
            Particle::remove(PARTICLE_KQ_AIR_NATTACK, 0);
            Particle::push(PARTICLE_KQ_AIR_NATTACK_GROUND,
                           -38, 0, -92, 60,
                           1.0f, 1.0f, this);
        }
    }

    if (airNAttackSprite->currentFrameX >= 9 * airNAttackSprite->width && y < DEFAULT_Y) {
        moveSpriteFrameX(KQ_AIR_NATTACK_SPRITE, 8 * airNAttackSprite->width);
    }
}

void Keqing::airDash() {
    xVelocity = KQ_AIR_DASH_SPEED;
    if (!facingEast) xVelocity = -xVelocity;
}

void Keqing::damage(int dt) { // TODO change
    if (!isDamaged()) {
        setSpriteAnimated(KQ_HURT_SPRITE, true);
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
            setSpriteAnimated(KQ_TURN_DASH_SPRITE, true);
        } else {
            setSpriteAnimated(KQ_TURN_SPRITE, true);
        }
        facingEast = value;
    }
}

bool Keqing::canMove() {
    return (canDoAction(KQ_WALK_SPRITE) ||
            isDashing() ||
            isAirDashing());
}

bool Keqing::canDoAction(int spriteCode) {
    if (spriteCode == KQ_WALK_SPRITE) {
        for (int i = KQ_WALK_SPRITE + 1; i < KQ_END_SPRITE_ENUM; i++) {
            if (isSameSpriteCode(&spriteArray[i], KQ_JUMP_SPRITE)) continue;
            if (spriteArray[i].animated) return false;
        }
        return true;
    }

    if (spriteArray[spriteCode].animated) return false;

    bool res = true;
    if (spriteCode == KQ_JUMP_START_SPRITE) {
        res = !spriteArray[KQ_STARWARD_SWORD_SPRITE].animated;
        if (!res) return res;
    }

    // By Default
    for (int i = spriteCode + 1; i < KQ_END_SPRITE_ENUM; i++) {
        if (spriteArray[i].animated) {
            res = false;
            break;
        }
    }
    return res;
}

void Keqing::preAction(int spriteCode) {
//    if (spriteCode == KQ_STARWARD_SWORD_SPRITE) {
//        for (int i = KQ_IDLE_SPRITE + 1; i < KQ_END_SPRITE_ENUM; i++) {
//            setSpriteAnimated(i, false);
//        }
//        return;
//    }
}

void Keqing::destroy() {
    AnimatedEntity::destroy();
    delete this;
}
