//
// Created by pyrowildx on 13/05/23.
//

#include <cstdio>
#include <vector>
#include <SDL2/SDL_image.h>
#include "Keqing.hpp"
#include "Particle.hpp"

#define MAX_TIME_BETWEEN_NATK 400
#define NATK_SPRITE_WIDTH 192

#define SKILL_TP_DISTANCE 400

#define BURST_FRAME_DURATION 70

Keqing *Keqing::instance = nullptr;

Keqing::Keqing(WindowRenderer *window)
        : AnimatedEntity(KQ_ENUM_N) {
    hp = 1;
    yVelocity = KQ_BASE_JUMP_VELOCITY;
    lastNAttackTime = 0;

    spriteArray[KQ_IDLE] =
            {KQ_IDLE, true,
             window->loadTexture("res/gfx/keqing/Idle.png"),
             0, 0, -36,
             96, 96,
             18 * 96, 60,
             0, 60,
             &spriteArray[KQ_IDLE]};

    spriteArray[KQ_JUMP_END] =
            {KQ_JUMP_END, false,
             window->loadTexture("res/gfx/keqing/JumpEnd.png"),
             0, 0, -34,
             96, 96,
             2 * 96, 100,
             0, 0,
             nullptr};

    spriteArray[KQ_WALK] =
            {KQ_WALK, false,
             window->loadTexture("res/gfx/keqing/Walk.png"),
             -10, 0, -24,
             96, 96,
             8 * 96, 60,
             0, 0,
             &spriteArray[KQ_WALK]};

    spriteArray[KQ_TURN] =
            {KQ_TURN, false,
             window->loadTexture("res/gfx/keqing/Turn.png"),
             -16, 0, -20,
             96, 96,
             3 * 96, 40,
             0, 0,
             nullptr};

    spriteArray[KQ_NATK] =
            {KQ_NATK, false,
             window->loadTexture("res/gfx/keqing/NAtk.png"),
             -38, -32, -92,
             NATK_SPRITE_WIDTH, 160,
             34 * NATK_SPRITE_WIDTH, 60,
             0, 0,
             nullptr};

    spriteArray[KQ_DASH] =
            {KQ_DASH, false,
             window->loadTexture("res/gfx/keqing/Dash.png"),
             -26, 0, -36,
             128, 96,
             9 * 128, 30,
             0, 0,
             nullptr};

    spriteArray[KQ_SKILL] =
            {KQ_SKILL, false,
             window->loadTexture("res/gfx/keqing/Skill.png"),
             -26, 0, -40,
             128, 96,
             13 * 128, 60,
             0, 0,
             nullptr};

    spriteArray[KQ_SKILL_SLASH] =
            {KQ_SKILL_SLASH, false,
             window->loadTexture("res/gfx/keqing/SkillSlash.png"),
             -48, -16, -84,
             192, 128,
             14 * 192, 60,
             0, 0,
             nullptr};

    spriteArray[KQ_BURST] =
            {KQ_BURST, false,
             window->loadTexture("res/gfx/keqing/Burst.png"),
             -24, 0, -44,
             128, 96,
             30 * 128, BURST_FRAME_DURATION,
             0, 0,
             nullptr};

    spriteArray[KQ_JUMP_START] =
            {KQ_JUMP_START, false,
             window->loadTexture("res/gfx/keqing/JumpStart.png"),
             0, 0, -36,
             96, 96,
             2 * 96, 20,
             0, 0,
             &spriteArray[KQ_JUMP]};

    spriteArray[KQ_JUMP] =
            {KQ_JUMP, false,
             window->loadTexture("res/gfx/keqing/Jump.png"),
             0, -34, -34,
             96, 128,
             7 * 96, 200,
             0, 0,
             &spriteArray[KQ_JUMP_END]};

    spriteArray[KQ_AIR_DASH] =
            {KQ_AIR_DASH, false,
             window->loadTexture("res/gfx/keqing/AirDash.png"),
             -10, 0, -26,
             96, 96,
             6 * 96, 40,
             0, 0,
             nullptr};

    spriteArray[KQ_AIR_NATK] =
            {KQ_AIR_NATK, false,
             window->loadTexture("res/gfx/keqing/AirNAtk.png"),
             -30, -32, -38,
             128, 128,
             14 * 128, 60,
             0, 0,
             nullptr};

    spriteArray[KQ_HURT] =
            {KQ_HURT, false,
             window->loadTexture("res/gfx/keqing/Hurt.png"),
             0, 0, 0,
             96, 96,
             6 * 96, 60,
             0, 0,
             nullptr};

    texture = spriteArray[KQ_IDLE].texture;
}

void Keqing::initKeqing(WindowRenderer *window) {
    if (instance == nullptr) {
        instance = new Keqing(window);
    } else {
        printf("Keqing already initialized!\n");
    }
}

void Keqing::colorTexture(int r, int g, int b, WindowRenderer *window) {
    SDL_Surface *img = IMG_Load("res/gfx/keqing/Idle.png");

    auto *pixel0 = (Uint8 *) img->pixels;
    for (Uint8 *pixel = pixel0; pixel < pixel0 + (img->w * img->h) * 4; pixel += 4) {

        float h, s, v;
        RGBtoHSV(pixel[0], pixel[1], pixel[2],
                 &h, &s, &v);

        // Hair (Green)
        if (h >= 120 && h <= 180) {
            h = 280 + (h - 120);
        }

        // Outfit & Gloves (Red)
        if ((h >= 330 && h <= 360) || (h >= 0 && h <= 30)) {
            if (h >= 330 && h <= 360)
                h = 280 + (h - 330);
            else if (h >= 0 && h <= 30)
                h = 280 + (h - 0);
        }

        HSVtoRGB(h, s, v,
                 &pixel[0], &pixel[1], &pixel[2]);
    }

    SDL_Texture *mTexture = SDL_CreateTextureFromSurface(
            window->getRenderer(), img);
    spriteArray[KQ_IDLE].texture = mTexture;
}




void Keqing::updateDirection(const bool *pressedKeys, int lastKey) {
    if (pressedKeys[KEY_Q] && pressedKeys[KEY_D]) {
        setFacingEast(!facingEast);
        xVelocity = 0;
    } else {
        if (pressedKeys[KEY_Q]) {
            setFacingEast(false);
            xVelocity = KQ_WALK_VELOCITY;
        } else if (pressedKeys[KEY_D]) {
            setFacingEast(true);
            xVelocity = KQ_WALK_VELOCITY;
        } else {
            xVelocity = 0;
        }
    }

    if (xVelocity != 0) {
        setSpriteAnimated(KQ_WALK, true);
    } else {
        setSpriteAnimated(KQ_WALK, false);
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
}

bool Keqing::shouldNotMove() {
    return (spriteArray[KQ_NATK].animated ||
            spriteArray[KQ_SKILL].animated ||
            spriteArray[KQ_SKILL_SLASH].animated ||
            spriteArray[KQ_BURST].animated ||
            spriteArray[KQ_AIR_NATK].animated);
}




const int numberOfNAtk = 5;
const int nAtkWidths[numberOfNAtk] = {0,
                                      4 * NATK_SPRITE_WIDTH,
                                      11 * NATK_SPRITE_WIDTH,
                                      18 * NATK_SPRITE_WIDTH,
                                      27 * NATK_SPRITE_WIDTH};
const int nAtkDashesX[numberOfNAtk] = {60, 0, 100,
                                       0, 400};

void Keqing::nAtk(int dt, int currentTime) {
    int lastTime = lastNAttackTime;
    lastNAttackTime = currentTime;
    Sprite *nAtkSprite = &spriteArray[KQ_NATK];
    //TODO IF A UNE FRAME PAS BON ALORS CANCEL SINON CONTINUER
    if (currentTime - lastTime < MAX_TIME_BETWEEN_NATK) {
        int i;
        for (i = 1; i < numberOfNAtk; i++) {
            if (nAtkSprite->frameX == nAtkWidths[i]) {
                setSpriteAnimated(KQ_NATK, false, false);
                nAtkSprite->frameX += nAtkSprite->width;
                return;
            }
            if (nAtkSprite->frameX < nAtkWidths[i]) break;
        }
        i--;
        float dashXToAdd = (float) (nAtkDashesX[i] * dt) * 0.001f;
        x += dashXToAdd;

        if (isNewestFrame(nAtkSprite,
                          nAtkWidths[4] + nAtkSprite->width)) { // NAtk 4
            Particle::push(PARTICLE_KQ_NATK_4,
                           32, 20, 60,
                           2.4f, 2.6f, this);
        }
    } else {
        nAtkSprite->frameX = 0;
        nAtkSprite->accumulatedTime = 0;
        return;
    }
}




void Keqing::dash(bool *pressedKeys) {
    xVelocity = -KQ_DASH_VELOCITY;
}




static void skillTimerHudOnRemove(Particle *particle) {
    Particle *skillIcon1Hud = Particle::getParticle(PARTICLE_HUD_SKILL_ICON_1, 0);
    skillIcon1Hud->setRGBAMod(255, 255, 255, 255);
    particle->getNextParticle(0)->setRGBAMod(KQ_SKILL_CIRCLE_RGBA);
}

int skillUseTime;

static void skillIcon2HudOnRemove(Particle *particle) {
    Particle *skillCircleHud = Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE, 0);

    Particle *skillIcon1 =
            Particle::push(PARTICLE_HUD_SKILL_ICON_1,
                           0, 0, INT32_MAX,
                           HUB_SB_ICON_M, HUB_SB_ICON_M,
                           nullptr);
    skillIcon1->moveToEntityCenter(skillCircleHud);
    skillIcon1->setRGBAMod(255, 255, 255, HUD_SB_USED_ALPHA);

    int elapsedTime = getTime() - skillUseTime;
    Particle *timerHud =
            Particle::push(PARTICLE_HUD_SKILL_BURST_TIMER,
                           0, 0,
                           (KQ_SKILL_COOLDOWN - elapsedTime) / HUD_SB_TIMER_FRAME_N,
                           HUD_SB_CIRCLE_M * 1.0f,
                           HUD_SB_CIRCLE_M * 1.0f,
                           nullptr);
    timerHud->moveToEntityCenter(skillCircleHud);
    Particle *newSkillCircleHud = skillCircleHud->copy();
    newSkillCircleHud->setRGBAMod(KQ_SKILL_CIRCLE_RGBA);
    timerHud->addNextParticle(newSkillCircleHud);
    timerHud->setOnRemove(&skillTimerHudOnRemove);

    Particle::remove(PARTICLE_HUD_SKILL_CIRCLE, 0);

    Particle::remove(PARTICLE_KQ_SKILL_IDLE, 0);
}

void Keqing::skill() {
    Sprite *skillSprite = &spriteArray[KQ_SKILL];
    if (skillSprite->animated) {
        if (isNewestFrame(skillSprite, 0)) {
            skillUseTime = getTime();

            Particle *skillCircleHud = Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE, 0);
            Particle *skillIcon2 =
                    Particle::push(PARTICLE_HUD_SKILL_ICON_2,
                                   0, 0,
                                   KQ_LIGHTNING_STILETTO_DURATION,
                                   HUB_SB_ICON_M, HUB_SB_ICON_M,
                                   nullptr);
            skillIcon2->moveToEntityCenter(skillCircleHud);
            skillIcon2->setOnRemove(&skillIcon2HudOnRemove);
            Particle::remove(PARTICLE_HUD_SKILL_ICON_1, 0);
            skillCircleHud->setRGBAMod(238, 10, 238, 255);

        } else if (isNewestFrame(skillSprite, 6 * skillSprite->width)) { // Lightning Stiletto
            Particle *spawnParticle =
                    Particle::push(PARTICLE_KQ_SKILL_SPAWN,
                                   0, 0, 60,
                                   1.0f, 1.0f, this);
            spawnParticle->setEntityDependant(false);
            float distance = SKILL_TP_DISTANCE;
            if (!facingEast) distance = -distance;
            spawnParticle->moveTo(this, distance, 0);
        }
    }

    Particle *spawnParticle = Particle::getParticle(PARTICLE_KQ_SKILL_SPAWN, 0);
    if (spawnParticle != nullptr) {
        Sprite *spawnParticleSprite = &spawnParticle->getSpriteArray()[0];
        if (isNewestFrame(spawnParticleSprite, 6 * spawnParticleSprite->width)) {
            Particle *idleParticle =
                    Particle::push(PARTICLE_KQ_SKILL_IDLE,
                                   0, 0, 200,
                                   2.0f, 2.0f, this);
            idleParticle->addNextParticle(idleParticle);
            idleParticle->setEntityDependant(false);
            float vX, vY;
            idleParticle->getToEntityCenterXY(spawnParticle,
                                              &vX, &vY);
            int newXShift = (int) (vX - spawnParticle->getX());
            int newYShift = (int) (vY - spawnParticle->getY());
            idleParticle->moveTo(spawnParticle);
            idleParticle->setXYShift(newXShift, newYShift, newXShift);
        }
    }
}




void Keqing::skillSlash() {
    Sprite *skillSlashSprite = &spriteArray[KQ_SKILL_SLASH];
    if (skillSlashSprite->animated) {
        if (isNewestFrame(skillSlashSprite, 0)) {
            Particle *idleParticle = Particle::getParticle(PARTICLE_KQ_SKILL_IDLE, 0);
            if (x < idleParticle->getX()) setFacingEast(true);
            else setFacingEast(false);

        } else if (isNewestFrame(skillSlashSprite, 6 * skillSlashSprite->width)) {
            Particle *tpStartParticle =
                    Particle::push(PARTICLE_KQ_BURST_VANISH,
                                   0, 12, 60,
                                   1.0f, 1.0f, this);
            tpStartParticle->setEntityDependant(false);
            tpStartParticle->moveTo(this);

            Particle *idleParticle = Particle::getParticle(PARTICLE_KQ_SKILL_IDLE, 0);
            this->moveTo(idleParticle);

            Particle::push(PARTICLE_KQ_SKILL_TP_END,
                           0, 0, 60,
                           0.64f, 0.64f, this);

            Particle::remove(PARTICLE_HUD_SKILL_ICON_2, 0);
        }
    }
}




const int cSlashFrameDuration = 16;
const int cSlashRotations[KQ_BURST_NUMBER_OF_CLONE_SLASH] =
        {16, 200, 354, 152, 306, 90};
const int cSlashXShift[KQ_BURST_NUMBER_OF_CLONE_SLASH] =
        {72, 0, -4, -10, -78, 0};
const int cSlashYShift[KQ_BURST_NUMBER_OF_CLONE_SLASH] =
        {26, 0, -72, 0, -30, -14};
const float cSlashWM[KQ_BURST_NUMBER_OF_CLONE_SLASH] =
        {0.88f, 1.6f, 1.2f, 1.52f, 1.32f, 1.0f};
const float cSlashHM[KQ_BURST_NUMBER_OF_CLONE_SLASH] =
        {1.0f, 1.2f, 1.2f, 1.2f, 1.2f, 1.2f};

static void pushCloneSlashParticle(Entity *keqing) {
    Particle *cSlashParticle =
            Particle::push(PARTICLE_KQ_BURST_CLONE_SLASH,
                           cSlashXShift[0],
                           cSlashYShift[0],
                           cSlashFrameDuration,
                           cSlashWM[0],
                           cSlashHM[0],
                           keqing);
    cSlashParticle->setRotation(cSlashRotations[0]);

    Particle *lastParticle = cSlashParticle;
    for (int i = 1; i < KQ_BURST_NUMBER_OF_CLONE_SLASH; i++) {
        auto *nextCSlashParticle = new Particle(PARTICLE_KQ_BURST_CLONE_SLASH,
                                                cSlashXShift[i],
                                                cSlashYShift[i],
                                                cSlashFrameDuration,
                                                cSlashWM[i],
                                                cSlashHM[i],
                                                keqing);
        nextCSlashParticle->setRotation(cSlashRotations[i]);
        lastParticle->addNextParticle(nextCSlashParticle);
        lastParticle = nextCSlashParticle;
    }
}

const int slashDuration = 46;
const int slashRotations[KQ_BURST_NUMBER_OF_SLASH] =
        {0, 20, 40, 60, 80, 100, 120, 140};

static void pushSlashParticle(Entity *keqing) {
    for (int i = 0; i < KQ_BURST_NUMBER_OF_SLASH; i++) {
        Particle *slashParticle =
                Particle::push(PARTICLE_KQ_BURST_SLASH,
                               0, 0, slashDuration,
                               1.2f, 1.2f, keqing);
        slashParticle->setEntityDependant(false);
        Particle *aoeParticle = Particle::getParticle(PARTICLE_KQ_BURST_AOE, 0);
        slashParticle->moveToEntityCenter(aoeParticle);
        slashParticle->setRotation(slashRotations[i]);
        slashParticle->delay(0, 2 * slashDuration * i);
    }
}

const int cVanishDuration = 40;
const int cVanishXShift[KQ_BURST_NUMBER_OF_CLONE] =
        {100, -80, 74, -100, -14};
const int cVanishYShift[KQ_BURST_NUMBER_OF_CLONE] =
        {26, -36, -48, 36, -80};

static void pushCloneVanishParticle(Entity *keqing, Particle *aoeParticle) {
    aoeParticle = Particle::getParticle(PARTICLE_KQ_BURST_CLONE, 0);
    for (int i = 0; i < KQ_BURST_NUMBER_OF_CLONE; i++) {
        Particle *cloneVanishParticle =
                Particle::push(PARTICLE_KQ_BURST_CLONE_VANISH,
                               cVanishXShift[i],
                               cVanishYShift[i],
                               cVanishDuration,
                               1.2f, 1.2f,
                               keqing);
        cloneVanishParticle->setEntityDependant(false);
        cloneVanishParticle->moveToEntityCenter(aoeParticle);
    }
}

static void burstTimerHudOnRemove(Particle *particle) {
    Particle *burstIcon = Particle::getParticle(PARTICLE_HUD_BURST_ICON, 0);
    burstIcon->setRGBAMod(255, 255, 255, 255);
}

const float aoeBaseWHM = 0.0f;
const float aoeMaxWHM = 2.0f;
const int cAppearDuration = 48;
const int cAppearXShift[KQ_BURST_NUMBER_OF_CLONE] =
        {142, -120, 98, -136, -16};
const int cAppearYShift[KQ_BURST_NUMBER_OF_CLONE] =
        {42, -40, -58, 58, -110};

void Keqing::burst(int dt) {
    Sprite *burstSprite = &spriteArray[KQ_BURST];
    if (isNewestFrame(burstSprite, 0)) { // Burst Start
        setFacingEast(true);

        Particle *burstCircleHud = Particle::getParticle(
                PARTICLE_HUD_BURST_CIRCLE, 0);
        Particle *timerHud =
                Particle::push(PARTICLE_HUD_SKILL_BURST_TIMER,
                               0, 0,
                               KQ_BURST_COOLDOWN / HUD_SB_TIMER_FRAME_N,
                               HUD_SB_CIRCLE_M * 1.486f,
                               HUD_SB_CIRCLE_M * 1.486f,
                               nullptr);
        timerHud->moveToEntityCenter(burstCircleHud);
        Particle *newBurstCircleHud = burstCircleHud->copy();
        timerHud->addNextParticle(newBurstCircleHud);
        timerHud->setOnRemove(&burstTimerHudOnRemove);
        Particle::remove(PARTICLE_HUD_BURST_CIRCLE, 0);
        Particle *burstIconHud = Particle::getParticle(PARTICLE_HUD_BURST_ICON, 0);
        burstIconHud->setRGBAMod(255, 255, 255, HUD_SB_USED_ALPHA);

        Particle *aoeParticle =
                Particle::push(PARTICLE_KQ_BURST_AOE,
                               0, 0, 80,
                               aoeBaseWHM, aoeBaseWHM,
                               this);
        aoeParticle->setEntityDependant(false);
        aoeParticle->addNextParticle(aoeParticle);

        Particle *aoeWaveParticle =
                Particle::push(PARTICLE_KQ_BURST_AOE_WAVE,
                               0, 0, 60,
                               aoeBaseWHM, aoeBaseWHM,
                               this);
        aoeWaveParticle->setEntityDependant(false);
        aoeWaveParticle->setRGBAMod(255, 255, 255, 128);
        aoeWaveParticle->addNextParticle(aoeWaveParticle);

    } else if (isNewestFrame(burstSprite, 12 * burstSprite->width)) { // Vanish
        Particle::push(PARTICLE_KQ_BURST_VANISH,
                       0, 0, 60,
                       1.0f, 1.0f, this);

    } else if (isNewestFrame(burstSprite, 13 * burstSprite->width)) { // Clone Slash Start
        pushCloneSlashParticle(this);
        Particle *cloneParticle =
                Particle::push(PARTICLE_KQ_BURST_CLONE,
                               0, -46,
                               (int) (cSlashFrameDuration * 1.8f),
                               2.0f, 2.0f, this);
        cloneParticle->delay(0, cSlashFrameDuration);
        cloneParticle->setEntityDependant(false);
        cloneParticle->moveToEntityCenter(nullptr);
        cloneParticle->setStopOnLastFrame(true);
        burstSprite->frameDuration = INT32_MAX;

    } else if (isNewestFrame(burstSprite, 28 * burstSprite->width)) { // Final Slash
        Particle *aoeParticle = Particle::getParticle(PARTICLE_KQ_BURST_AOE, 0);
        Particle *aoeWaveParticle = Particle::getParticle(PARTICLE_KQ_BURST_AOE_WAVE, 0);

        Particle *finalSlashParticle =
                Particle::push(PARTICLE_KQ_BURST_FINAL_SLASH,
                               0, 0, 80,
                               1.0f, 1.0f, this);
        finalSlashParticle->setEntityDependant(false);
        finalSlashParticle->moveToEntityCenter(aoeParticle);

        pushCloneVanishParticle(this, aoeParticle);

        aoeParticle->fadeAway();
        aoeWaveParticle->fadeAway();

        Particle::remove(PARTICLE_KQ_BURST_CLONE, 0);
    }

    // Enlarge Circle
    Particle *aoeParticle = Particle::getParticle(PARTICLE_KQ_BURST_AOE, 0);
    if (aoeParticle != nullptr) {
        if (aoeParticle->getRenderWMultiplier() < aoeMaxWHM) {
            float addWHValue = 0.004f * (float) dt;
            aoeParticle->addRenderWHMultiplier(addWHValue, addWHValue,
                                               aoeMaxWHM, aoeMaxWHM);
            aoeParticle->moveToEntityCenter(nullptr);
            Particle *aoeWaveParticle = Particle::getParticle(PARTICLE_KQ_BURST_AOE_WAVE, 0);
            aoeWaveParticle->addRenderWHMultiplier(addWHValue, addWHValue,
                                                   aoeMaxWHM, aoeMaxWHM);
            aoeWaveParticle->moveToEntityCenter(nullptr);
        }
    }

    if (burstSprite->frameX == 13 * burstSprite->width) {
        Particle *cloneParticle = Particle::getParticle(PARTICLE_KQ_BURST_CLONE, 0);
        Sprite *cloneParticleSprite = cloneParticle->getSpriteArray();
        for (int i = 0; i < KQ_BURST_NUMBER_OF_CLONE; i++) {
            // 0, 5, 10, 15, 20
            if (isNewestFrame(cloneParticleSprite, (5 * i) * cloneParticleSprite->width)) {
                Particle *cloneAppearParticle =
                        Particle::push(PARTICLE_KQ_BURST_CLONE_APPEAR,
                                       cAppearXShift[i],
                                       cAppearYShift[i],
                                       cAppearDuration,
                                       1.0f, 1.0f,
                                       this);
                cloneAppearParticle->setEntityDependant(false);
                cloneAppearParticle->moveToEntityCenter(nullptr);
            }
        }

        if (!Particle::isActive(PARTICLE_KQ_BURST_CLONE_SLASH, 0)) { // Clone Slash End
            burstSprite->frameDuration = BURST_FRAME_DURATION;
            burstSprite->accumulatedTime = BURST_FRAME_DURATION;
            pushSlashParticle(this);
        }
    }
}




void Keqing::jump(int dt) {
    y -= yVelocity * (float) dt;

    yVelocity -= (float) dt * 0.0016f;

    if (y > 360) { // TODO
        y = 360;
        yVelocity = KQ_BASE_JUMP_VELOCITY;
        setSpriteAnimated(KQ_JUMP, false);
        if (spriteArray[KQ_JUMP].next != nullptr) {
            spriteArray[KQ_JUMP].next->animated = true;
        }
    }
}




void Keqing::airNAtk(int dt) { // Plunge Attack in Genshin
    Sprite *airNAttackSprite = &spriteArray[KQ_AIR_NATK];

    if (isNewestFrame(airNAttackSprite, 0)) {
        setSpriteAnimated(KQ_JUMP, false);
        yVelocity = 0.0f;
    } else if (isNewestFrame(airNAttackSprite, 1 * airNAttackSprite->width)) {
        yVelocity = 0.4f;
    } else if (isNewestFrame(airNAttackSprite, 6 * airNAttackSprite->width)) {
        Particle *airNAttackParticle =
                Particle::push(PARTICLE_KQ_AIR_NATK,
                               24, -24, 60,
                               3.2f, 2.0f, this);
        airNAttackParticle->addNextParticle(airNAttackParticle);
    }

    if (airNAttackSprite->frameX < 9 * airNAttackSprite->width) {
        y -= yVelocity * (float) dt;

        yVelocity -= (float) dt * 0.002f;

        if (y > 360) {
            y = 360;
            yVelocity = KQ_BASE_JUMP_VELOCITY;
            moveSpriteFrameX(KQ_AIR_NATK, 9 * airNAttackSprite->width);
            Particle::remove(PARTICLE_KQ_AIR_NATK, 0);
            Particle::push(PARTICLE_KQ_AIR_NATK_GROUND,
                           20, -6, 60,
                           1.0f, 1.0f, this);
        }
    }

    if (airNAttackSprite->frameX >= 9 * airNAttackSprite->width && y < 360) {
        moveSpriteFrameX(KQ_AIR_NATK, 8 * airNAttackSprite->width);
    }
}




void Keqing::airDash() {
    xVelocity = KQ_AIR_DASH_VELOCITY;
}




void Keqing::damage(int dt) { // TODO change
    if (!isDamaged()) {
        setSpriteAnimated(KQ_HURT, true);
        x -= 10;
        hp--;
    } else {
        x -= KQ_KNOCKBACK_VELOCITY * (float) dt;
    }
}




void Keqing::setFacingEast(bool value) {
    if (facingEast != value) {
        setSpriteAnimated(KQ_TURN, true);
        facingEast = value;
    }
}




static bool checkNextSprites(int spriteCode, Sprite *spriteArray) {
    for (int i = spriteCode + 1; i < KQ_ENUM_N; i++) {
        if (spriteArray[i].animated) return false;
    }
    return true;
}

bool Keqing::canDoAction(int spriteCode) {
    if (spriteCode == KQ_WALK) {
        for (int i = KQ_WALK + 1; i < KQ_ENUM_N; i++) {
            if (isSameSpriteCode(&spriteArray[i], KQ_JUMP)) continue;
            if (spriteArray[i].animated) return false;
        }
        return true;
    }

    if (spriteArray[spriteCode].animated) return false;

    if (spriteCode == KQ_SKILL) {
        Particle *skillSpawnParticle = Particle::getParticle(PARTICLE_KQ_SKILL_SPAWN, 0);
        if (skillSpawnParticle != nullptr) return false;
        if (Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE, 0) == nullptr) return false;

    } else if (spriteCode == KQ_BURST) {
        if (spriteArray[KQ_SKILL].animated) return false;
        if (spriteArray[KQ_SKILL_SLASH].animated) return false;
        if (Particle::getParticle(PARTICLE_HUD_BURST_CIRCLE, 0) == nullptr) return false;

    } else if (spriteCode == KQ_JUMP_START) {
        if (spriteArray[KQ_DASH].animated) return false;
        if (spriteArray[KQ_SKILL].animated) return false;
        if (spriteArray[KQ_SKILL_SLASH].animated) return false;
        if (spriteArray[KQ_BURST].animated) return false;
    }

    // By Default
    return checkNextSprites(spriteCode, spriteArray);
}

void Keqing::preAction(int spriteCode, const bool *pressedKeys) {
    // TODO BASICALLY PUT EVERY ISNEWESTFRAME 0 HERE
    if (spriteCode == KQ_DASH) {
        if (pressedKeys[KEY_Q]) setFacingEast(true);
        else if (pressedKeys[KEY_D]) setFacingEast(false);
    }
}

void Keqing::destroy() {
    AnimatedEntity::destroy();
    delete this;
}
