//
// Created by pyrowildx on 13/05/23.
//

#include <cmath>
#include <SDL2/SDL_image.h>
#include "Keqing.hpp"
#include "Entity/Particle.hpp"
#include "Utils/Global.hpp"

#define SKILL_TP_DISTANCE 400

Keqing *Keqing::instance = nullptr;
int Keqing::spriteXShifts[KQ_ENUM_N];
int Keqing::spriteYShifts[KQ_ENUM_N];
int Keqing::spriteXRShifts[KQ_ENUM_N];

const int NAtkMax = 4;
const int NAtkEndFrame[NAtkMax] = {3, 9, 17, 28};

void Keqing::setXYShift(int spriteCode, int xShift, int yShift, int xRShift) {
    spriteXShifts[spriteCode] = xShift;
    spriteYShifts[spriteCode] = yShift;
    spriteXRShifts[spriteCode] = xRShift;
}

Keqing::Keqing()
        : AnimatedEntity(KQ_ENUM_N) {
    gravityWeight = 0.0024;
    hp = 1;
    airDashed = false;

    initSprite(KQ_IDLE, "res/gfx/keqing/Idle.png",
               96, 96, 18, 60);
    setXYShift(KQ_IDLE, 0, 0, -36);
    Keqing::setSpriteAnimated(KQ_IDLE, true);
    setSpriteNext(KQ_IDLE, KQ_IDLE);

    initSprite(KQ_RUN_END, "res/gfx/keqing/RunEnd.png",
               96, 96, 5, 60);
    setXYShift(KQ_RUN_END, 0, 0, -36);

    initSprite(KQ_JUMP_END, "res/gfx/keqing/JumpEnd.png",
               96, 96, 2, 100);
    setXYShift(KQ_JUMP_END, 0, 0, -34);

    initSprite(KQ_WALK, "res/gfx/keqing/Walk.png",
               96, 96, 8, 60);
    setXYShift(KQ_WALK, -12, 0, -24);
    setSpriteNext(KQ_WALK, KQ_WALK);

    initSprite(KQ_WALK_TURN, "res/gfx/keqing/WalkTurn.png",
               96, 96, 3, 40);
    setXYShift(KQ_WALK_TURN, -16, 0, -20);

    initSprite(KQ_RUN_START, "res/gfx/keqing/RunStart.png",
               96, 96, 3, 60);
    setXYShift(KQ_RUN_START, -4, 0, -32);
    setSpriteNext(KQ_RUN_START, KQ_RUN);

    initSprite(KQ_RUN, "res/gfx/keqing/Run.png",
               128, 96, 8, 60);
    setXYShift(KQ_RUN, -32, 0, -36);
    setSpriteNext(KQ_RUN, KQ_RUN);

    initSprite(KQ_RUN_TURN, "res/gfx/keqing/RunTurn.png",
               96, 96, 3, 60);
    setXYShift(KQ_RUN_TURN, 0, 0, 0);

    initSprite(KQ_JUMP_START, "res/gfx/keqing/JumpStart.png",
               96, 96, 2, 20);
    setXYShift(KQ_JUMP_START, 0, 0, -36);
    setSpriteNext(KQ_JUMP_START, KQ_JUMP);

    initSprite(KQ_JUMP, "res/gfx/keqing/Jump.png",
               96, 128, 7, 100);
    setXYShift(KQ_JUMP, 0, -34, -34);
    setSpriteFrameLengthFromTo(KQ_JUMP, INT32_MAX, 2, 4);
    setSpriteNext(KQ_JUMP, KQ_JUMP_END);

    initSprite(KQ_NATK, "res/gfx/keqing/NAtk.png",
               192, 160, 34, 60);
    setXYShift(KQ_NATK, -38, -32, -92);

    initSprite(KQ_CATK, "res/gfx/keqing/CAtk.png",
               128, 96, 15, 60);
    setXYShift(KQ_CATK, -34, 0, -32);

    initSprite(KQ_DASH, "res/gfx/keqing/Dash.png",
               128, 96, 9, 30);
    setXYShift(KQ_DASH, -24, 0, -42);
    setSpriteFrameLengthFromTo(KQ_DASH, 42, 2, 4); // Invisible
    setSpriteFrameLengthFromTo(KQ_DASH, 54, 6, -1); // End

    initSprite(KQ_SKILL, "res/gfx/keqing/Skill.png",
               128, 96, 13, 60);
    setXYShift(KQ_SKILL, -26, 0, -40);

    initSprite(KQ_SKILL_AIMING, "res/gfx/keqing/SkillAiming.png",
               128, 96, 8, INT32_MAX);
    setXYShift(KQ_SKILL_AIMING, -26, 0, -40);

    initSprite(KQ_SKILL_SLASH, "res/gfx/keqing/SkillSlash.png",
               192, 128, 14, 60);
    setXYShift(KQ_SKILL_SLASH, -48, -16, -84);

    initSprite(KQ_BURST, "res/gfx/keqing/Burst.png",
               128, 96, 30, 70);
    setXYShift(KQ_BURST, -24, 0, -44);
    setSpriteFrameLengthFromTo(KQ_BURST, INT32_MAX, 13);

    initSprite(KQ_AIR_NATK, "res/gfx/keqing/AirNAtk.png",
               128, 128, 14, 60);
    setXYShift(KQ_AIR_NATK, -30, -32, -38);

    initSprite(KQ_AIR_DASH, "res/gfx/keqing/AirDash.png",
               96, 96, 6, 40);
    setXYShift(KQ_AIR_DASH, -10, 2, -26);

    initSprite(KQ_HURT, "res/gfx/keqing/Hurt.png",
               96, 96, 6, 60);
    setXYShift(KQ_HURT, 0, 0, 0);
}


void Keqing::initKeqing() {
    if (instance == nullptr) {
        instance = new Keqing();
    } else {
        SDL_Log("Keqing already initialized!\n");
    }
}


void Keqing::colorTexture(Uint8 r, Uint8 g, Uint8 b) {
    SDL_Renderer *gRenderer = WindowRenderer::getInstance()->getRenderer();

    for (int spriteCode = 0; spriteCode < KQ_ENUM_N; spriteCode++) {
        Sprite *currSprite = getSprite(spriteCode);
        SDL_Surface *img = IMG_Load(currSprite->imgPath);

        auto *pixel0 = (Uint8 *) img->pixels;
        for (Uint8 *pixel = pixel0; pixel < pixel0 + (img->w * img->h) * 4; pixel += 4) {

            double h, s, v;
            RGBtoHSV(pixel[0], pixel[1], pixel[2],
                     &h, &s, &v);

            double rgbH, rgbS, rgbV;
            RGBtoHSV(r, g, b,
                     &rgbH, &rgbS, &rgbV);


            bool change = false;
            // Hair (Green)
            if (h >= 120 && h <= 180) {
                h = rgbH + (h - 120);

                change = true;
            }

            // Outfit & Gloves (Red)
            if ((h >= 330 && h <= 360) || (h >= 0 && h <= 30)) {
                if (h >= 330 && h <= 360)
                    h = rgbH + (h - 330);
                else if (h >= 0 && h <= 30)
                    h = rgbH + (h - 0);

                change = true;
            }

            if (change)
                HSVtoRGB(h, rgbS, v,
                         &pixel[0], &pixel[1], &pixel[2]);
        }

        SDL_Texture *mTexture = SDL_CreateTextureFromSurface(gRenderer, img);
        currSprite->sTexture = mTexture;
    }
}


void Keqing::moveX() {
    if (!isSpriteAnimated(KQ_NATK) &&
        !isSpriteAnimated(KQ_CATK) &&
        !isSpriteAnimated(KQ_DASH)) {
        Entity::moveX();

    } else {
        double addX = xVelocity * (double) Global::dt;
        if (!facingEast) addX = -addX;

        double vX = x + addX;
        if (addX >= 0) vX += hitBox.x + hitBox.w;
        double yDown = y + hitBox.y + hitBox.h;
        if (Global::currentWorld->isPixelSurface(vX, yDown)) {
            Entity::moveX();
        }
    }
}


SDL_Rect Keqing::getRenderRect() {
    SDL_Rect dst = Entity::getRenderRect();

    double xCoeff, yCoeff;
    getScreenXYCoeff(&xCoeff, &yCoeff);
    if (facingEast) dst.x += (int) ((double) spriteXShifts[getCurrentSpriteCode()] * xCoeff);
    else dst.x += (int) ((double) spriteXRShifts[getCurrentSpriteCode()] * xCoeff);

    dst.y += (int) ((double) spriteYShifts[getCurrentSpriteCode()] * yCoeff);

    return dst;
}


void Keqing::setSpriteAnimated(int spriteCode, bool animated) {
    if (animated && !isSpriteAnimated(spriteCode)) {
        preAction(spriteCode);
    }
    AnimatedEntity::setSpriteAnimated(spriteCode, animated);
}


bool Keqing::shouldUpdateDirection() {
    if (isCurrentSprite(KQ_WALK) ||
        isCurrentSprite(KQ_RUN) ||
        isCurrentSprite(KQ_JUMP)) {
        return true;
    }

    if (isCurrentSprite(KQ_NATK)) {
        for (int i = 0; i < NAtkMax; i++) {
            if (isFrameBetween(KQ_NATK, NAtkEndFrame[i], NAtkEndFrame[i] + 1)) {
                return true;
            }
        }
    }

    return false;
}

void Keqing::updateDirection() {
    if (isKeyPressed(KEY_Q) && isKeyPressed(KEY_D)) {
        setFacingEast(!facingEast);
    } else {
        if (isKeyPressed(KEY_Q) || isKeyPressed(KEY_D)) {
            setFacingEast(!isKeyPressed(KEY_Q));
        }
    }
}

bool Keqing::canMoveLR() {
    return (isCurrentSprite(KQ_WALK) ||
            isCurrentSprite(KQ_RUN) ||
            isCurrentSprite(KQ_JUMP));
}

void Keqing::moveLR() {
    if (isKeyPressed(KEY_Q) && isKeyPressed(KEY_D)) {
        xVelocity = 0;
        setSpriteAnimated(KQ_WALK, false);
        setSpriteAnimated(KQ_RUN, false);
    } else {
        if (isKeyPressed(KEY_Q) || isKeyPressed(KEY_D)) {
            if (isCurrentSprite(KQ_JUMP)) {
                xVelocity = (facingEast) ? KQ_WALK_VELOCITY : -KQ_WALK_VELOCITY;
                if (isKeyPressed(KEY_Q)) xVelocity = -xVelocity;
            } else if (isCurrentSprite(KQ_RUN)) {
                xVelocity = KQ_RUN_VELOCITY;
            } else if (isCurrentSprite(KQ_WALK)) {
                xVelocity = KQ_WALK_VELOCITY;
            }
        } else {
            xVelocity = 0;
            setSpriteAnimated(KQ_WALK, false);
            setSpriteAnimated(KQ_RUN, false);
            if (isSpriteAnimated(KQ_RUN)) {
                setSpriteAnimated(KQ_RUN_END, true);
            }
        }
    }
}

void Keqing::jump() {
    if (isSpriteAnimated(KQ_JUMP_START)) {
        if (willFrameFinish(KQ_JUMP_START, -1)) {
            yVelocity = -KQ_JUMP_BASE_VELOCITY;
        }
    }

    if (isSpriteAnimated(KQ_JUMP)) {
        if (isFrameBetween(KQ_JUMP, 0, 3)) {
            yVelocity += gravityWeight * (double) Global::dt;
            yVelocity -= gravityWeight * (double) Global::dt; // Cancel Gravity
        }
    }
}


const double NAtkVelocity[NAtkMax + 1] = {0.1, 0, 0.2,
                                          0, 0.4};

void Keqing::NAtk() {
    for (int i = 0; i < NAtkMax; i++) {
        if (willFrameFinish(KQ_NATK, NAtkEndFrame[i] + 1)) {
            if (!isMouseLeftRecent()) { // Stop NAtk
                setSpriteAnimated(KQ_NATK, false);
                if (isMouseLeftLong()) {
                    setSpriteAnimated(KQ_CATK, true);
                }
                return;
            }
        }

        if (isFrameBetween(KQ_NATK, NAtkEndFrame[i], NAtkEndFrame[i] + 1)) {
            xVelocity = 0;
            if (isMouseLeftRecent()) { // Continue NAtk
                goToFrame(KQ_NATK, NAtkEndFrame[i] + 2);
                xVelocity = NAtkVelocity[i + 1];
            }
        }
    }

    if (isNewestFrame(KQ_NATK, 0)) {
        xVelocity = NAtkVelocity[0];

    } else if (isNewestFrame(KQ_NATK, 30)) { // NAtk 4
        Particle *NAtk4Particle =
                Particle::push(PARTICLE_KQ_NATK_4,
                               60, 2.4, 2.6);
        NAtk4Particle->setEntity(this);
        NAtk4Particle->xyShift(32, 20);
    }
}


void Keqing::CAtk() {
    if (isNewestFrame(KQ_CATK, 0)) {
        xVelocity = -0.4;

    } else if (isNewestFrame(KQ_CATK, 6)) {
        Particle *CAtkParticle =
                Particle::push(PARTICLE_KQ_CATK, 60);
        CAtkParticle->moveToEntityCenter(this);
        CAtkParticle->xyShift(30, 10);
    }

    if (xVelocity < 0) {
        xVelocity = std::min(xVelocity + 0.001 * Global::dt, 0.0);
    }
}


void Keqing::dash() {
    if (isNewestFrame(KQ_DASH, 0)) {
        if (isKeyPressed(KEY_Q)) setFacingEast(true);
        else if (isKeyPressed(KEY_D)) setFacingEast(false);

    } else if (isNewestFrame(KQ_DASH, 2)) {
        xVelocity = -KQ_DASH_VELOCITY;

    } else if (isFrameBetween(KQ_DASH, 6, -1)) {
        if (xVelocity < 0) {
            xVelocity = std::min(xVelocity + 0.01 * (double) Global::dt, 0.0);
        }
    }
}


Particle *newSkillCircleHud = nullptr;

static void skillTimerHudOnRemove(Particle *removedParticle) {
    Particle::pushFast(newSkillCircleHud);
    newSkillCircleHud->setRGBAMod(KQ_SKILL_CIRCLE_RGBA);
    Particle *skillIcon1Hud = Particle::getParticle(PARTICLE_HUD_SKILL_ICON_1, 0);
    skillIcon1Hud->setRGBAMod(RGBA_FULL);
}

int skillUseTime;

static void skillIcon2HudOnRemove(Particle *removedParticle) {
    Particle *skillCircleHud = Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE, 0);
    newSkillCircleHud = skillCircleHud->copy();
    newSkillCircleHud->setRGBAMod(KQ_SKILL_CIRCLE_RGBA);

    Particle *skillIcon1 =
            Particle::push(PARTICLE_HUD_SKILL_ICON_1,
                           INT32_MAX,
                           HUB_SB_ICON_M, HUB_SB_ICON_M);
    skillIcon1->moveToEntityCenter(skillCircleHud);
    skillIcon1->setRGBAMod(RGB_FULL, HUD_SB_USED_ALPHA);

    int elapsedTime = getTime() - skillUseTime;
    Particle *timerHud =
            Particle::push(PARTICLE_HUD_SKILL_BURST_TIMER,
                           (KQ_SKILL_COOLDOWN - elapsedTime) / HUD_SB_TIMER_FRAME_N,
                           HUD_SB_CIRCLE_M * 1.0,
                           HUD_SB_CIRCLE_M * 1.0);
    timerHud->moveToEntityCenter(skillCircleHud);
    timerHud->setOnRemove(&skillTimerHudOnRemove);

    Particle::remove(PARTICLE_HUD_SKILL_CIRCLE, 0);

    Particle::remove(PARTICLE_KQ_SKILL_IDLE, 0);
}

double lightningStelittoXY[2];

static void createLightningStelitto(Keqing *kq, int mouseX = -1, int mouseY = -1) {
    Particle *skillCircleHud = Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE, 0);
    Particle *skillIcon2 =
            Particle::push(PARTICLE_HUD_SKILL_ICON_2,
                           KQ_LIGHTNING_STILETTO_DURATION,
                           HUB_SB_ICON_M, HUB_SB_ICON_M);
    skillIcon2->moveToEntityCenter(skillCircleHud);
    skillIcon2->setOnRemove(&skillIcon2HudOnRemove);
    Particle::remove(PARTICLE_HUD_SKILL_ICON_1, 0);
    skillCircleHud->setRGBAMod(238, 10, 238, 255);

    Particle *spawnParticle =
            Particle::push(PARTICLE_KQ_SKILL_SPAWN, 60);
    if (mouseX == -1 || mouseY == -1) {
        spawnParticle->moveToEntityCenter(kq);
        double distance = SKILL_TP_DISTANCE;
        if (!kq->isFacingEast()) distance = -distance;
        spawnParticle->moveTo(kq, distance, 0);
        lightningStelittoXY[0] = kq->getX() + distance;
        lightningStelittoXY[1] = kq->getY();
    } else {
        spawnParticle->moveToCenter(mouseX, mouseY);
        lightningStelittoXY[0] = mouseX;
        lightningStelittoXY[1] = mouseY;
    }

    skillUseTime = getTime();
}

void Keqing::ESkill() {
    if (isNewestFrame(KQ_SKILL, 4)) { // Lightning Stiletto
        if (!isKeyPressed(KEY_E)) {
            createLightningStelitto(this);
        } else {
            pauseSprite(KQ_SKILL, true);
            setSpriteAnimated(KQ_SKILL_AIMING, true);
        }
    }

    Particle *spawnParticle = Particle::getParticle(PARTICLE_KQ_SKILL_SPAWN, 0);
    if (spawnParticle != nullptr) { // Lightning Stiletto Idle
        if (spawnParticle->isNewestFrame(PARTICLE_KQ_SKILL_SPAWN, 6)) {
            Particle *idleParticle =
                    Particle::push(PARTICLE_KQ_SKILL_IDLE,
                                   200, 2.0, 2.0);
            idleParticle->moveToEntityCenter(spawnParticle);
        }
    }
}


void Keqing::ESkillAiming() {
    Particle *cursorIdleParticle = Particle::getParticle(PARTICLE_KQ_SKILL_AIMING_IDLE, 0);
    if (cursorIdleParticle == nullptr) {
        cursorIdleParticle =
                Particle::push(PARTICLE_KQ_SKILL_AIMING_IDLE,
                               60, 2.0, 2.0);
    }
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    getMouseAbsoluteXY(&mouseX, &mouseY);
    mouseX += Global::currentWorld->getBackground()->getFrame().x;
    cursorIdleParticle->moveToCenter(mouseX, mouseY);

    double kqCenterX, kqCenterY;
    getSelfCenter(&kqCenterX, &kqCenterY);
    double xDiff = abs(mouseX - kqCenterX);
    double yDiff = abs(mouseY - kqCenterY);
    double angle;
    if ((mouseX > kqCenterX && mouseY < kqCenterY) || (mouseX < kqCenterX && mouseY > kqCenterY)) {
        angle = atan(yDiff / xDiff);
    } else {
        angle = atan(xDiff / yDiff);
    }
    if (mouseX < kqCenterX && mouseY > kqCenterY) angle += M_PI;
    else if (mouseX < kqCenterX) angle += M_PI / 2.0;
    else if (mouseY > kqCenterY) angle += (3.0 * M_PI) / 2.0;
    goToFrame(KQ_SKILL_AIMING, (int) ((angle / (2.0 * M_PI)) * 8));

    if (!isKeyPressed(KEY_E)) {
        Particle::remove(PARTICLE_KQ_SKILL_AIMING_IDLE, 0);
        setSpriteAnimated(KQ_SKILL_AIMING, false);
        createLightningStelitto(this, mouseX, mouseY);
        pauseSprite(KQ_SKILL, false);
        goToFrame(KQ_SKILL, 5);
        return;
    }
}


void Keqing::ESkillSlash() {
    if (isNewestFrame(KQ_SKILL_SLASH, 0)) {
        Particle *idleParticle = Particle::getParticle(PARTICLE_KQ_SKILL_IDLE, 0);
        if (x <= idleParticle->getX()) setFacingEast(true);
        else setFacingEast(false);

    } else if (isNewestFrame(KQ_SKILL_SLASH, 6)) {
        Particle *tpStartParticle =
                Particle::push(PARTICLE_KQ_BURST_VANISH, 60);
        tpStartParticle->moveToEntityCenter(this);

        this->moveTo(lightningStelittoXY[0], lightningStelittoXY[1]);

        Particle *tpEndParticle = Particle::push(PARTICLE_KQ_SKILL_TP_END,
                                                 60, 0.64, 0.64);
        tpEndParticle->moveToEntityCenter(this);

        Particle::remove(PARTICLE_HUD_SKILL_ICON_2, 0);
    }
}


const int cSlashFrameDuration = 16;
const int cSlashRotations[KQ_BURST_NUMBER_OF_CLONE_SLASH] =
        {16, 200, 354, 152, 306, 90};
const double cSlashXShift[KQ_BURST_NUMBER_OF_CLONE_SLASH] =
        {72, 0, -4, -10, -78, 0};
const double cSlashYShift[KQ_BURST_NUMBER_OF_CLONE_SLASH] =
        {26, 0, -72, 0, -30, -14};
const double cSlashWM[KQ_BURST_NUMBER_OF_CLONE_SLASH] =
        {0.88, 1.6, 1.2, 1.52, 1.32, 1.0};
const double cSlashHM[KQ_BURST_NUMBER_OF_CLONE_SLASH] =
        {1.0, 1.2, 1.2, 1.2, 1.2, 1.2};
int cSlashCount = 0;

static void pushCloneSlashParticle(Particle *removedParticle) {
    if (cSlashCount < KQ_BURST_NUMBER_OF_CLONE_SLASH) {
        auto *cSlashParticle =
                Particle::push(PARTICLE_KQ_BURST_CLONE_SLASH,
                               cSlashFrameDuration,
                               cSlashWM[cSlashCount],
                               cSlashHM[cSlashCount]);
        cSlashParticle->moveToEntityCenter(Keqing::getInstance());
        cSlashParticle->xyShift(cSlashXShift[cSlashCount], cSlashYShift[cSlashCount]);
        cSlashParticle->setRotation(cSlashRotations[cSlashCount]);
        cSlashParticle->setOnRemove(&pushCloneSlashParticle);
        cSlashCount++;
    } else {
        cSlashCount = 0;
    }
}

const int slashDuration = 46;
const int slashRotations[KQ_BURST_NUMBER_OF_SLASH] =
        {0, 20, 40, 60, 80, 100, 120, 140};

static void pushSlashParticle() {
    Particle *aoeParticle = Particle::getParticle(PARTICLE_KQ_BURST_AOE, 0);
    for (int i = 0; i < KQ_BURST_NUMBER_OF_SLASH; i++) {
        Particle *slashParticle =
                Particle::push(PARTICLE_KQ_BURST_SLASH,
                               slashDuration,
                               1.2, 1.2);
        slashParticle->moveToEntityCenter(aoeParticle);
        slashParticle->setRotation(slashRotations[i]);
        slashParticle->delaySprite(PARTICLE_KQ_BURST_SLASH, 2 * slashDuration * i);
    }
}

const int cVanishDuration = 40;
const double cVanishXShift[KQ_BURST_NUMBER_OF_CLONE] =
        {100, -80, 74, -100, -14};
const double cVanishYShift[KQ_BURST_NUMBER_OF_CLONE] =
        {26, -36, -48, 36, -80};

static void pushCloneVanishParticle(Particle *aoeParticle) {
    aoeParticle = Particle::getParticle(PARTICLE_KQ_BURST_CLONE, 0);
    for (int i = 0; i < KQ_BURST_NUMBER_OF_CLONE; i++) {
        Particle *cloneVanishParticle =
                Particle::push(PARTICLE_KQ_BURST_CLONE_VANISH,
                               cVanishDuration,
                               1.2, 1.2);
        cloneVanishParticle->moveToEntityCenter(aoeParticle);
        cloneVanishParticle->xyShift(cVanishXShift[i], cVanishYShift[i]);
    }
}

const double aoeBaseWHM = 0;
const double aoeMaxWHM = 2.0;
const int cAppearDuration = 48;
const double cAppearXShift[KQ_BURST_NUMBER_OF_CLONE] =
        {142, -120, 98, -136, -16};
const double cAppearYShift[KQ_BURST_NUMBER_OF_CLONE] =
        {42, -40, -58, 58, -110};

void Keqing::RBurst() {
    if (isNewestFrame(KQ_BURST, 0)) { // Burst Start
        Particle::remove(PARTICLE_HUD_BURST_CIRCLE, 0);
        Particle *burstIconHud = Particle::getParticle(PARTICLE_HUD_BURST_ICON, 0);
        burstIconHud->setRGBAMod(RGB_FULL, HUD_SB_USED_ALPHA);

        Particle::push(PARTICLE_KQ_BURST_AOE,
                       80, aoeBaseWHM, aoeBaseWHM);

        Particle *aoeWaveParticle =
                Particle::push(PARTICLE_KQ_BURST_AOE_WAVE,
                               60, aoeBaseWHM, aoeBaseWHM);
        aoeWaveParticle->setRGBAMod(RGB_FULL, 128);

    } else if (isNewestFrame(KQ_BURST, 12)) { // Vanish
        Particle *vanishParticle =
                Particle::push(PARTICLE_KQ_BURST_VANISH, 60);
        vanishParticle->setEntity(this);

    } else if (isNewestFrame(KQ_BURST, 13)) { // Clone Slash Start
        pushCloneSlashParticle(nullptr);
        Particle *cloneParticle =
                Particle::push(PARTICLE_KQ_BURST_CLONE,
                               (int) (cSlashFrameDuration * 1.8),
                               2.0, 2.0);
        cloneParticle->delaySprite(PARTICLE_KQ_BURST_CLONE, cSlashFrameDuration);
        cloneParticle->moveToEntityCenter(this);
        cloneParticle->xyShift(0, -46);
        cloneParticle->stopOnFrame(cloneParticle->getCode());

    } else if (isNewestFrame(KQ_BURST, 24)) { // Final Slash
        Particle *aoeParticle = Particle::getParticle(PARTICLE_KQ_BURST_AOE, 0);
        Particle *aoeWaveParticle = Particle::getParticle(PARTICLE_KQ_BURST_AOE_WAVE, 0);

        Particle *finalSlashParticle =
                Particle::push(PARTICLE_KQ_BURST_FINAL_SLASH, 80);
        finalSlashParticle->moveToEntityCenter(aoeParticle);

        pushCloneVanishParticle(aoeParticle);

        aoeParticle->fadeAway();
        aoeWaveParticle->fadeAway();

        Particle::remove(PARTICLE_KQ_BURST_CLONE, 0);
    }

    // Enlarge Circle
    Particle *aoeParticle = Particle::getParticle(PARTICLE_KQ_BURST_AOE, 0);
    if (aoeParticle != nullptr) {
        if (aoeParticle->getRenderWMultiplier() < aoeMaxWHM) {
            double addWHValue = 0.004 * (double) Global::dt;
            aoeParticle->addRenderWHMultiplier(addWHValue, addWHValue,
                                               aoeMaxWHM, aoeMaxWHM);
            aoeParticle->moveToEntityCenter(this);
            Particle *aoeWaveParticle = Particle::getParticle(PARTICLE_KQ_BURST_AOE_WAVE, 0);
            aoeWaveParticle->addRenderWHMultiplier(addWHValue, addWHValue,
                                                   aoeMaxWHM, aoeMaxWHM);
            aoeWaveParticle->moveToEntityCenter(this);
        }
    }

    if (isFrameAt(KQ_BURST, 13)) {
        Particle *cloneParticle = Particle::getParticle(PARTICLE_KQ_BURST_CLONE, 0);
        for (int i = 0; i < KQ_BURST_NUMBER_OF_CLONE; i++) {
            // 0, 5, 10, 15, 20
            if (cloneParticle->isNewestFrame(PARTICLE_KQ_BURST_CLONE, 5 * i)) {
                Particle *cloneAppearParticle =
                        Particle::push(PARTICLE_KQ_BURST_CLONE_APPEAR,
                                       cAppearDuration);
                cloneAppearParticle->moveToEntityCenter(this);
                cloneAppearParticle->xyShift(cAppearXShift[i], cAppearYShift[i]);
            }
        }

        if (!Particle::isActive(PARTICLE_KQ_BURST_CLONE_SLASH, 0)) { // Clone Slash End
            goToNextFrame(KQ_BURST);
            pushSlashParticle();
        }
    }
}


const double lowFallVelocity = 0.1;
const double averageFallVelocity = 0.2;

void Keqing::airAnimate() {
    if (yVelocity == 0 && !isInAir()) {
        if (isSpriteAnimated(KQ_JUMP)) {
            setSpriteAnimated(KQ_JUMP, false);
            setSpriteAnimated(KQ_JUMP_END, true);
            airDashed = false;
        }
        return;
    }

    setSpriteAnimated(KQ_JUMP, true);

    if (yVelocity < 0) {
        if (yVelocity < -averageFallVelocity) {
            if (willFrameFinish(KQ_JUMP, 1)) {
                goToFrame(KQ_JUMP, 0);
            }
        } else if (yVelocity < -lowFallVelocity) {
            goToFrame(KQ_JUMP, 2);
        } else {
            goToFrame(KQ_JUMP, 3);
        }
    }

    if (yVelocity > 0) {
        if (yVelocity < lowFallVelocity) {
            goToFrame(KQ_JUMP, 3);
        } else if (yVelocity < averageFallVelocity) {
            goToFrame(KQ_JUMP, 4);
        } else {
            if (isFrameBetween(KQ_JUMP, 0, 4)) {
                goToFrame(KQ_JUMP, 5);
            }
            if (willFrameFinish(KQ_JUMP, -1)) {
                goToFrame(KQ_JUMP, 5);
            }
        }
    }
}


void Keqing::airNAtk() { // Plunge Attack in Genshin
    if (isNewestFrame(KQ_AIR_NATK, 0)) {
        setSpriteAnimated(KQ_JUMP, false);

    } else if (isNewestFrame(KQ_AIR_NATK, 1)) {
        yVelocity = -0.4;

    } else if (isNewestFrame(KQ_AIR_NATK, 6)) {
        Particle *airNAttackParticle =
                Particle::push(PARTICLE_KQ_AIR_NATK,
                               60, 3.2, 0.1);
        airNAttackParticle->setEntity(this);
        airNAttackParticle->xyShift(26, 52);

        yVelocity = 0.2;
    }

    Particle *airNAttackPaticle =
            Particle::getParticle(PARTICLE_KQ_AIR_NATK, 0);
    if (airNAttackPaticle != nullptr) {
        const double maxRenderH = 2.0;
        if (airNAttackPaticle->getRenderHMultiplier() < maxRenderH) {
            airNAttackPaticle->addRenderWHMultiplierR(0, 0.02 * Global::dt,
                                                      0, maxRenderH);
        }
    }

    if (willFrameFinish(KQ_AIR_NATK, 7)) {
        if (yVelocity != 0) {
            goToFrame(KQ_AIR_NATK, 6);
        }
    }

    if (isFrameBetween(KQ_AIR_NATK, 6, 7)) {
        if (yVelocity == 0) {
            goToFrame(KQ_AIR_NATK, 8);
            Particle::remove(PARTICLE_KQ_AIR_NATK, 0);
            Particle *airNAtkGroundParticle =
                    Particle::push(PARTICLE_KQ_AIR_NATK_GROUND, 60);
            airNAtkGroundParticle->moveToEntityCenter(this);
            airNAtkGroundParticle->xyShift(20, -6);
        }
    }

}

void Keqing::airDash() {
    yVelocity = 0;

    if (isNewestFrame(KQ_AIR_DASH, 0)) {
        airDashed = true;
        if (isKeyPressed(KEY_Q)) {
            setFacingEast(true);
            xVelocity = -KQ_AIR_DASH_VELOCITY;
        } else if (isKeyPressed(KEY_D)) {
            setFacingEast(false);
            xVelocity = -KQ_AIR_DASH_VELOCITY;
        } else xVelocity = 0;
    }
}


void Keqing::damage() {
    // TODO
}


void Keqing::setFacingEast(bool value) {
    if (facingEast != value) {
        if (!isSpriteAnimated(KQ_RUN_START) &&
            !isSpriteAnimated(KQ_RUN) &&
            !isSpriteAnimated(KQ_RUN_TURN)) {
            setSpriteAnimated(KQ_WALK_TURN, true);
        } else {
            setSpriteAnimated(KQ_RUN_TURN, true);
        }
        facingEast = value;
    }
}

bool Keqing::canDoAction(int spriteCode) {
    switch (spriteCode) {
        case KQ_WALK:
            if (isSpriteAnimated(KQ_RUN)) {
                setSpriteAnimated(KQ_RUN, false);
                return true;
            }
            break;

        case KQ_SKILL:
            if (Particle::getParticle(PARTICLE_KQ_SKILL_SPAWN, 0) != nullptr) return false;
            if (Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE, 0) == nullptr) return false;
            break;

        case KQ_BURST:
            if (isSpriteAnimated(KQ_SKILL)) return false;
            if (isSpriteAnimated(KQ_SKILL_SLASH)) return false;
            if (Particle::getParticle(PARTICLE_HUD_BURST_CIRCLE, 0) == nullptr) return false;
            break;

        case KQ_AIR_DASH:
            if (airDashed) return false;
            if (isSpriteAnimated(KQ_AIR_NATK)) return false;
            break;

        default:
            break;
    }

    if (isSpriteAnimated(spriteCode)) return false;

    for (int i = spriteCode + 1; i < KQ_ENUM_N; i++) {
        if (isSpriteAnimated(i)) return false;
    }
    return true;
}

void Keqing::preAction(int spriteCode) {
    if (spriteCode != KQ_WALK &&
        spriteCode != KQ_WALK_TURN &&
        spriteCode != KQ_RUN_START &&
        spriteCode != KQ_RUN &&
        spriteCode != KQ_RUN_TURN &&
        spriteCode != KQ_RUN_END &&
        spriteCode != KQ_JUMP_START &&
        spriteCode != KQ_JUMP &&
        spriteCode != KQ_JUMP_END) {
        xVelocity = 0;
    }
}

void Keqing::updateAction() {
    if (isSpriteAnimated(KQ_JUMP_START) ||
        isSpriteAnimated(KQ_JUMP)) {
        this->jump();
    }
    if (isSpriteAnimated(KQ_NATK)) this->NAtk();
    if (isSpriteAnimated(KQ_CATK)) this->CAtk();
    if (isSpriteAnimated(KQ_DASH)) this->dash();
    if (isSpriteAnimated(KQ_SKILL)) this->ESkill();
    if (isSpriteAnimated(KQ_SKILL_AIMING)) this->ESkillAiming();
    if (isSpriteAnimated(KQ_SKILL_SLASH)) this->ESkillSlash();
    if (isSpriteAnimated(KQ_BURST)) this->RBurst();
    if (isSpriteAnimated(KQ_AIR_NATK)) this->airNAtk();
    if (isSpriteAnimated(KQ_AIR_DASH)) this->airDash();
    if (isSpriteAnimated(KQ_HURT)) this->damage();
}
