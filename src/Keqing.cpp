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

void Keqing::setXYShift(int xShift, int yShift, int xRShift, int spriteCode) {
    spriteXShifts[spriteCode] = xShift;
    spriteYShifts[spriteCode] = yShift;
    spriteXRShifts[spriteCode] = xRShift;
}

Keqing::Keqing()
        : AnimatedEntity(KQ_ENUM_N) {
    gravityWeight = 0.0024;
    hp = 1;
    jumpPressTime = 0;
    doubleJumped = false;
    airDashed = false;

    initSprite(KQ_IDLE, "res/gfx/keqing/Idle.png",
               96, 96, 18, 60);
    setXYShift(0, 0, -36, KQ_IDLE);
    setSpriteNext(KQ_IDLE, KQ_IDLE);

    initSprite(KQ_RUN_END, "res/gfx/keqing/RunEnd.png",
               96, 96, 5, 60);
    setXYShift(0, 0, -36, KQ_RUN_END);

    initSprite(KQ_JUMP_END, "res/gfx/keqing/JumpEnd.png",
               96, 96, 2, 100);
    setXYShift(0, 0, -34, KQ_JUMP_END);

    initSprite(KQ_WALK, "res/gfx/keqing/Walk.png",
               96, 96, 8, 60);
    setXYShift(-12, 0, -24, KQ_WALK);
    setSpriteNext(KQ_WALK, KQ_WALK);

    initSprite(KQ_WALK_TURN, "res/gfx/keqing/WalkTurn.png",
               96, 96, 3, 40);
    setXYShift(-16, 0, -20, KQ_WALK_TURN);

    initSprite(KQ_RUN_START, "res/gfx/keqing/RunStart.png",
               96, 96, 3, 60);
    setXYShift(-4, 0, -32, KQ_RUN_START);
    setSpriteNext(KQ_RUN, KQ_RUN_START);

    initSprite(KQ_RUN, "res/gfx/keqing/Run.png",
               128, 96, 8, 60);
    setXYShift(-38, 0, -30, KQ_RUN);
    setSpriteNext(KQ_RUN, KQ_RUN);

    initSprite(KQ_RUN_TURN, "res/gfx/keqing/RunTurn.png",
               96, 96, 3, 60);
    setXYShift(-20, 0, -20, KQ_RUN_TURN);

    initSprite(KQ_CROUCH, "res/gfx/keqing/Crouch.png",
               96, 96, 5, 60);
    setSpriteFrameLengthFromTo(INT32_MAX, 2, 2, KQ_CROUCH);
    setXYShift(0, 0, -32, KQ_CROUCH);

    initSprite(KQ_NATK, "res/gfx/keqing/NAtk.png",
               192, 160, 34, 60);
    setXYShift(-38, -32, -92, KQ_NATK);

    initSprite(KQ_CATK, "res/gfx/keqing/CAtk.png",
               128, 96, 15, 60);
    setXYShift(-34, 0, -32, KQ_CATK);

    initSprite(KQ_DASH, "res/gfx/keqing/Dash.png",
               128, 96, 9, 30);
    setXYShift(-24, 0, -42, KQ_DASH);
    setSpriteFrameLengthFromTo(42, 2, 4, KQ_DASH); // Invisible
    setSpriteFrameLengthFromTo(54, 6, -1, KQ_DASH); // End

    initSprite(KQ_SKILL, "res/gfx/keqing/Skill.png",
               128, 96, 13, 60);
    setXYShift(-26, 0, -40, KQ_SKILL);

    initSprite(KQ_SKILL_AIMING, "res/gfx/keqing/SkillAiming.png",
               128, 96, 8, INT32_MAX);
    setXYShift(-26, 0, -40, KQ_SKILL_AIMING);

    initSprite(KQ_SKILL_SLASH, "res/gfx/keqing/SkillSlash.png",
               192, 128, 14, 60);
    setXYShift(-48, -16, -84, KQ_SKILL_SLASH);

    initSprite(KQ_BURST, "res/gfx/keqing/Burst.png",
               128, 96, 30, 70);
    setXYShift(-24, 0, -44, KQ_BURST);
    setSpriteFrameLengthFromTo(INT32_MAX, 13, 13, KQ_BURST);

    initSprite(KQ_JUMP_START, "res/gfx/keqing/JumpStart.png",
               96, 96, 2, 20);
    setXYShift(0, 0, -36, KQ_JUMP_START);
    setSpriteNext(KQ_JUMP, KQ_JUMP_START);

    initSprite(KQ_JUMP, "res/gfx/keqing/Jump.png",
               96, 128, 7, 100);
    setXYShift(0, -34, -34, KQ_JUMP);
    setSpriteFrameLengthFromTo(INT32_MAX, 2, 4, KQ_JUMP);
    setSpriteNext(KQ_JUMP_END, KQ_JUMP);

    initSprite(KQ_AIR_DOUBLE_JUMP, "res/gfx/keqing/AirDoubleJump.png",
               96, 96, 8, 40);
    setXYShift(-6, 0, -26, KQ_AIR_DOUBLE_JUMP);

    initSprite(KQ_AIR_NATK, "res/gfx/keqing/AirNAtk.png",
               160, 128, 11, 40);
    setXYShift(-48, -8, -48, KQ_AIR_NATK);

    initSprite(KQ_AIR_PLUNGE, "res/gfx/keqing/AirPlunge.png",
               128, 128, 14, 60);
    setXYShift(-30, -32, -38, KQ_AIR_PLUNGE);

    initSprite(KQ_AIR_DASH, "res/gfx/keqing/AirDash.png",
               96, 96, 6, 40);
    setXYShift(-10, 2, -26, KQ_AIR_DASH);

    initSprite(KQ_HURT, "res/gfx/keqing/Hurt.png",
               96, 96, 6, 60);
    setXYShift(0, 0, 0, KQ_HURT);
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


void Keqing::setSpriteAnimated(bool animated, int spriteCode) {
    AnimatedEntity::setSpriteAnimated(animated, spriteCode, &preAction, &xVelocity);
}


bool Keqing::shouldUpdateDirection() {
    if (isCurrentSprite(KQ_WALK) ||
        isCurrentSprite(KQ_RUN) ||
        isCurrentSprite(KQ_JUMP) ||
        isCurrentSprite(KQ_AIR_DOUBLE_JUMP) ||
        isCurrentSprite(KQ_AIR_NATK)) {
        return true;
    }

    if (isCurrentSprite(KQ_NATK)) {
        for (int i = 0; i < NAtkMax; i++) {
            if (isFrameBetween(NAtkEndFrame[i], NAtkEndFrame[i] + 1, KQ_NATK)) {
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
            isCurrentSprite(KQ_JUMP) ||
            isCurrentSprite(KQ_AIR_DOUBLE_JUMP) ||
            isCurrentSprite(KQ_AIR_NATK));
}

void Keqing::moveLR() {
    if (isKeyPressed(KEY_Q) && isKeyPressed(KEY_D)) {
        xVelocity = 0;
        setSpriteAnimated(false, KQ_WALK);
        setSpriteAnimated(false, KQ_RUN);
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
            setSpriteAnimated(false, KQ_WALK);
            setSpriteAnimated(false, KQ_RUN);
            if (isSpriteAnimated(KQ_RUN)) {
                setSpriteAnimated(true, KQ_RUN_END);
            }
        }
    }
}


void Keqing::crouch() {
    if (isFrameBetween(0, 2, KQ_CROUCH)) {
        if (!isKeyPressed(KEY_S)) {
            goToFrame(3, KQ_CROUCH);
        }
    }
}


const double NAtkVelocity[NAtkMax + 1] = {0.1, 0, 0.2,
                                          0, 0.4};

void Keqing::NAtk() {
    for (int i = 0; i < NAtkMax; i++) {
        if (willFrameFinish(NAtkEndFrame[i] + 1, KQ_NATK)) {
            if (!isMouseLeftRecent()) { // Stop NAtk
                setSpriteAnimated(false, KQ_NATK);
                if (isMouseLeftLong()) {
                    setSpriteAnimated(true, KQ_CATK);
                }
                return;
            }
        }

        if (isFrameBetween(NAtkEndFrame[i], NAtkEndFrame[i] + 1, KQ_NATK)) {
            xVelocity = 0;
            if (isMouseLeftRecent()) { // Continue NAtk
                goToFrame(NAtkEndFrame[i] + 2, KQ_NATK);
                xVelocity = NAtkVelocity[i + 1];
            }
        }
    }

    if (isNewestFrame(0, KQ_NATK)) {
        xVelocity = NAtkVelocity[0];

    } else if (isNewestFrame(30, KQ_NATK)) { // NAtk 4
        Particle *NAtk4Particle =
                Particle::push(PARTICLE_KQ_NATK_4,
                               60, 2.4, 2.6);
        NAtk4Particle->setEntity(this);
        NAtk4Particle->xyShift(32, 20);
    }
}


void Keqing::CAtk() {
    if (isNewestFrame(0, KQ_CATK)) {
        xVelocity = -0.4;

    } else if (isNewestFrame(6, KQ_CATK)) {
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
    if (isNewestFrame(0, KQ_DASH)) {
        if (isKeyPressed(KEY_Q)) setFacingEast(true);
        else if (isKeyPressed(KEY_D)) setFacingEast(false);

    } else if (isNewestFrame(2, KQ_DASH)) {
        xVelocity = -KQ_DASH_VELOCITY;

    } else if (isFrameBetween(6, -1, KQ_DASH)) {
        if (xVelocity < 0) {
            xVelocity = std::min(xVelocity + 0.01 * (double) Global::dt, 0.0);
        }
    }
}


Particle *newSkillCircleHud = nullptr;

static void skillTimerHudOnRemove(Particle *removedParticle) {
    Particle::pushFast(newSkillCircleHud);
    newSkillCircleHud->setRGBAMod(KQ_SKILL_CIRCLE_RGBA);
    Particle *skillIcon1Hud = Particle::getParticle(PARTICLE_HUD_SKILL_ICON_1);
    skillIcon1Hud->setRGBAMod(RGBA_FULL);
}

int skillUseTime;

static void skillIcon2HudOnRemove(Particle *removedParticle) {
    Particle *skillCircleHud = Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE);
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

    Particle::remove(PARTICLE_HUD_SKILL_CIRCLE);

    Particle::remove(PARTICLE_KQ_SKILL_IDLE);
}

double lightningStelittoXY[2];

static void createLightningStelitto(Keqing *kq, int mouseX = -1, int mouseY = -1) {
    Particle *skillCircleHud = Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE);
    Particle *skillIcon2 =
            Particle::push(PARTICLE_HUD_SKILL_ICON_2,
                           KQ_LIGHTNING_STILETTO_DURATION,
                           HUB_SB_ICON_M, HUB_SB_ICON_M);
    skillIcon2->moveToEntityCenter(skillCircleHud);
    skillIcon2->setOnRemove(&skillIcon2HudOnRemove);
    Particle::remove(PARTICLE_HUD_SKILL_ICON_1);
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
    if (isNewestFrame(4, KQ_SKILL)) { // Lightning Stiletto
        if (!isKeyPressed(KEY_E)) {
            createLightningStelitto(this);
        } else {
            pauseSprite(true, KQ_SKILL);
            setSpriteAnimated(true, KQ_SKILL_AIMING);
        }
    }

    Particle *spawnParticle =
            Particle::getParticle(PARTICLE_KQ_SKILL_SPAWN);
    if (spawnParticle != nullptr) { // Lightning Stiletto Idle
        if (spawnParticle->isNewestFrame(6)) {
            Particle *idleParticle =
                    Particle::push(PARTICLE_KQ_SKILL_IDLE,
                                   200, 2.0, 2.0);
            idleParticle->moveToEntityCenter(spawnParticle);
        }
    }
}

bool wasFacingEast;

void Keqing::ESkillAiming() {
    Particle *cursorIdleParticle =
            Particle::getParticle(PARTICLE_KQ_SKILL_AIMING_IDLE);
    if (cursorIdleParticle == nullptr) {
        cursorIdleParticle =
                Particle::push(PARTICLE_KQ_SKILL_AIMING_IDLE,
                               60, 2.0, 2.0);
        wasFacingEast = facingEast;
        setFacingEast(true);
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

    int frameIndex = (int) ((angle / (2.0 * M_PI)) * 8);
    goToFrame(frameIndex, KQ_SKILL_AIMING);

    if (!isKeyPressed(KEY_E)) {
        Particle::remove(PARTICLE_KQ_SKILL_AIMING_IDLE);
        setSpriteAnimated(false, KQ_SKILL_AIMING);
        setFacingEast(wasFacingEast);
        createLightningStelitto(this, mouseX, mouseY);
        pauseSprite(false, KQ_SKILL);
        goToFrame(5, KQ_SKILL);
        return;
    }
}


void Keqing::ESkillSlash() {
    yVelocity = 0;

    if (isNewestFrame(0, KQ_SKILL_SLASH)) {
        Particle *idleParticle =
                Particle::getParticle(PARTICLE_KQ_SKILL_IDLE);
        if (x <= idleParticle->getX()) setFacingEast(true);
        else setFacingEast(false);

    } else if (isNewestFrame(6, KQ_SKILL_SLASH)) {
        Particle *tpStartParticle =
                Particle::push(PARTICLE_KQ_BURST_VANISH, 60);
        tpStartParticle->moveToEntityCenter(this);

        this->moveTo(lightningStelittoXY[0], lightningStelittoXY[1]);

        Particle *tpEndParticle =
                Particle::push(PARTICLE_KQ_SKILL_TP_END,
                               60, 0.64, 0.64);
        tpEndParticle->moveToEntityCenter(this);

        // TODO MOVE TO ENTITY CENTER FOR KEQING WITH HITBOX RECT

        Particle::remove(PARTICLE_HUD_SKILL_ICON_2);
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
    Particle *aoeParticle =
            Particle::getParticle(PARTICLE_KQ_BURST_AOE);
    for (int i = 0; i < KQ_BURST_NUMBER_OF_SLASH; i++) {
        Particle *slashParticle =
                Particle::push(PARTICLE_KQ_BURST_SLASH,
                               slashDuration,
                               1.2, 1.2);
        slashParticle->moveToEntityCenter(aoeParticle);
        slashParticle->setRotation(slashRotations[i]);
        slashParticle->delaySprite(2 * slashDuration * i);
    }
}

const int cVanishDuration = 40;
const double cVanishXShift[KQ_BURST_NUMBER_OF_CLONE] =
        {100, -80, 74, -100, -14};
const double cVanishYShift[KQ_BURST_NUMBER_OF_CLONE] =
        {26, -36, -48, 36, -80};

static void pushCloneVanishParticle(Particle *aoeParticle) {
    aoeParticle =
            Particle::getParticle(PARTICLE_KQ_BURST_CLONE);
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
    if (isNewestFrame(0, KQ_BURST)) { // Burst Start
        Particle::remove(PARTICLE_HUD_BURST_CIRCLE);
        Particle *burstIconHud =
                Particle::getParticle(PARTICLE_HUD_BURST_ICON);
        burstIconHud->setRGBAMod(RGB_FULL, HUD_SB_USED_ALPHA);

        Particle::push(PARTICLE_KQ_BURST_AOE,
                       80, aoeBaseWHM, aoeBaseWHM);

        Particle *aoeWaveParticle =
                Particle::push(PARTICLE_KQ_BURST_AOE_WAVE,
                               60, aoeBaseWHM, aoeBaseWHM);
        aoeWaveParticle->setRGBAMod(RGB_FULL, 128);

    } else if (isNewestFrame(12, KQ_BURST)) { // Vanish
        Particle *vanishParticle =
                Particle::push(PARTICLE_KQ_BURST_VANISH, 60);
        vanishParticle->setEntity(this);

    } else if (isNewestFrame(13, KQ_BURST)) { // Clone Slash Start
        pushCloneSlashParticle(nullptr);
        Particle *cloneParticle =
                Particle::push(PARTICLE_KQ_BURST_CLONE,
                               (int) (cSlashFrameDuration * 1.8),
                               2.0, 2.0);
        cloneParticle->delaySprite(cSlashFrameDuration);
        cloneParticle->moveToEntityCenter(this);
        cloneParticle->xyShift(0, -46);
        cloneParticle->stopOnFrame(-1);

    } else if (isNewestFrame(24, KQ_BURST)) { // Final Slash
        Particle *aoeParticle =
                Particle::getParticle(PARTICLE_KQ_BURST_AOE);
        Particle *aoeWaveParticle =
                Particle::getParticle(PARTICLE_KQ_BURST_AOE_WAVE);

        Particle *finalSlashParticle =
                Particle::push(PARTICLE_KQ_BURST_FINAL_SLASH, 80);
        finalSlashParticle->moveToEntityCenter(aoeParticle);

        pushCloneVanishParticle(aoeParticle);

        aoeParticle->fadeAway();
        aoeWaveParticle->fadeAway();

        Particle::remove(PARTICLE_KQ_BURST_CLONE);
    }

    // Enlarge Circle
    Particle *aoeParticle =
            Particle::getParticle(PARTICLE_KQ_BURST_AOE);
    if (aoeParticle != nullptr) {
        if (aoeParticle->getRenderWMultiplier() < aoeMaxWHM) {
            double addWHValue = 0.004 * (double) Global::dt;
            aoeParticle->addRenderWHMultiplier(addWHValue, addWHValue,
                                               aoeMaxWHM, aoeMaxWHM);
            aoeParticle->moveToEntityCenter(this);
            Particle *aoeWaveParticle =
                    Particle::getParticle(PARTICLE_KQ_BURST_AOE_WAVE);
            aoeWaveParticle->addRenderWHMultiplier(addWHValue, addWHValue,
                                                   aoeMaxWHM, aoeMaxWHM);
            aoeWaveParticle->moveToEntityCenter(this);
        }
    }

    if (isFrameAt(13, KQ_BURST)) {
        Particle *cloneParticle =
                Particle::getParticle(PARTICLE_KQ_BURST_CLONE);
        for (int i = 0; i < KQ_BURST_NUMBER_OF_CLONE; i++) {
            // 0, 5, 10, 15, 20
            if (cloneParticle->isNewestFrame(5 * i)) {
                Particle *cloneAppearParticle =
                        Particle::push(PARTICLE_KQ_BURST_CLONE_APPEAR,
                                       cAppearDuration);
                cloneAppearParticle->moveToEntityCenter(this);
                cloneAppearParticle->xyShift(cAppearXShift[i], cAppearYShift[i]);
            }
        }

        if (!Particle::isActive(PARTICLE_KQ_BURST_CLONE_SLASH)) { // Clone Slash End
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
            setSpriteAnimated(false, KQ_JUMP);
            setSpriteAnimated(true, KQ_JUMP_END);
            doubleJumped = false;
            airDashed = false;
        }
        return;
    }

    setSpriteAnimated(true, KQ_JUMP);

    if (yVelocity < 0) {
        if (yVelocity < -averageFallVelocity) {
            if (willFrameFinish(1, KQ_JUMP)) {
                goToFrame(0, KQ_JUMP);
            }
        } else if (yVelocity < -lowFallVelocity) {
            goToFrame(2, KQ_JUMP);
        } else {
            goToFrame(3, KQ_JUMP);
        }
    }

    if (yVelocity > 0) {
        if (yVelocity < lowFallVelocity) {
            goToFrame(3, KQ_JUMP);
        } else if (yVelocity < averageFallVelocity) {
            goToFrame(4, KQ_JUMP);
        } else {
            if (isFrameBetween(0, 4, KQ_JUMP)) {
                goToFrame(5, KQ_JUMP);
            }
            if (willFrameFinish(-1, KQ_JUMP)) {
                goToFrame(5, KQ_JUMP);
            }
        }
    }
}


void Keqing::jump() {
    if (isNewestFrame(0, KQ_JUMP_START)) {
        jumpPressTime = Global::pressedTime[KEY_SPACE];
    }

    if (isSpriteAnimated(KQ_JUMP_START)) {
        if (willFrameFinish(-1, KQ_JUMP_START)) {
            yVelocity = -KQ_JUMP_BASE_VELOCITY;
        }
    }

    if (isSpriteAnimated(KQ_JUMP)) {
        if (isFrameBetween(0, 3, KQ_JUMP)) {
            yVelocity += gravityWeight * (double) Global::dt;
            yVelocity -= gravityWeight * (double) Global::dt; // Cancel Gravity
        }
    }
}


void Keqing::airDoubleJump() {
    if (isNewestFrame(0, KQ_AIR_DOUBLE_JUMP)) {
        doubleJumped = true;
        yVelocity = -KQ_AIR_DOUBLE_JUMP_BASE_VELOCITY;
    }
}


void Keqing::airNAtk() {
    if (yVelocity == 0) {
        setSpriteAnimated(false, KQ_AIR_NATK);
    }
}


void Keqing::airPlunge() { // Plunge Attack in Genshin
    if (isNewestFrame(0, KQ_AIR_PLUNGE)) {
        setSpriteAnimated(false, KQ_JUMP);

    } else if (isNewestFrame(1, KQ_AIR_PLUNGE)) {
        yVelocity = -0.4;

    } else if (isNewestFrame(6, KQ_AIR_PLUNGE)) {
        Particle *airNAttackParticle =
                Particle::push(PARTICLE_KQ_AIR_PLUNGE,
                               60, 3.2, 0.1);
        airNAttackParticle->setEntity(this);
        airNAttackParticle->xyShift(26, 52);

        yVelocity = 0.2;
    }

    Particle *airNAttackPaticle =
            Particle::getParticle(PARTICLE_KQ_AIR_PLUNGE);
    if (airNAttackPaticle != nullptr) {
        const double maxRenderH = 2.0;
        if (airNAttackPaticle->getRenderHMultiplier() < maxRenderH) {
            airNAttackPaticle->addRenderWHMultiplierR(0, 0.02 * Global::dt,
                                                      0, maxRenderH);
        }
    }

    if (willFrameFinish(7, KQ_AIR_PLUNGE)) {
        if (yVelocity != 0) {
            goToFrame(6, KQ_AIR_PLUNGE);
        }
    }

    if (isFrameBetween(6, 7, KQ_AIR_PLUNGE)) {
        if (yVelocity == 0) {
            goToFrame(8, KQ_AIR_PLUNGE);
            Particle::remove(PARTICLE_KQ_AIR_PLUNGE);
            Particle *airNAtkGroundParticle =
                    Particle::push(PARTICLE_KQ_AIR_PLUNGE_GROUND, 60);
            airNAtkGroundParticle->moveToEntityCenter(this);
            airNAtkGroundParticle->xyShift(20, -6);
        }
    }

}

void Keqing::airDash() {
    yVelocity = 0;

    if (isNewestFrame(0, KQ_AIR_DASH)) {
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
            setSpriteAnimated(true, KQ_WALK_TURN);
        } else {
            setSpriteAnimated(true, KQ_RUN_TURN);
        }
        facingEast = value;
    }
}

bool Keqing::canDoAction(int spriteCode) {
    bool spriteCodeToSkip[KQ_ENUM_N];
    std::fill(spriteCodeToSkip, spriteCodeToSkip + KQ_ENUM_N,
              false);

    switch (spriteCode) {
        case KQ_WALK:
            if (isCurrentSprite(KQ_RUN)) {
                setSpriteAnimated(false, KQ_RUN);
                return true;
            }
            break;

        case KQ_JUMP_START:
            if (isSpriteAnimated(KQ_CATK)) return false;
            if (isSpriteAnimated(KQ_SKILL)) return false;
            if (isSpriteAnimated(KQ_SKILL_AIMING)) return false;
            if (isSpriteAnimated(KQ_SKILL_SLASH)) return false;
            if (isSpriteAnimated(KQ_BURST)) return false;
            break;

        case KQ_SKILL:
            if (Particle::getParticle(PARTICLE_KQ_SKILL_SPAWN) != nullptr) return false;
            if (Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE) == nullptr) return false;
            break;

        case KQ_BURST:
            if (isSpriteAnimated(KQ_SKILL)) return false;
            if (isSpriteAnimated(KQ_SKILL_AIMING)) return false;
            if (isSpriteAnimated(KQ_SKILL_SLASH)) return false;
            if (Particle::getParticle(PARTICLE_HUD_BURST_CIRCLE) == nullptr) return false;
            break;

        case KQ_AIR_DOUBLE_JUMP:
            if (jumpPressTime == Global::pressedTime[KEY_SPACE]) return false;
            if (doubleJumped) return false;
            spriteCodeToSkip[KQ_AIR_NATK] = true;
            break;

        case KQ_AIR_DASH:
            if (airDashed) return false;
            if (isSpriteAnimated(KQ_AIR_PLUNGE)) return false;
            break;

        default:
            break;
    }

    if (isSpriteAnimated(spriteCode)) return false;

    for (int i = spriteCode + 1; i < KQ_ENUM_N; i++) {
        if (spriteCodeToSkip[i]) continue;
        if (isSpriteAnimated(i)) return false;
    }
    return true;
}

void Keqing::preAction(int spriteCode, void *fParams) {
    auto *pXVeclocity = (double *) fParams;
    if (spriteCode != KQ_WALK &&
        spriteCode != KQ_RUN_START &&
        spriteCode != KQ_RUN &&
        spriteCode != KQ_RUN_END &&
        spriteCode != KQ_JUMP_START &&
        spriteCode != KQ_JUMP &&
        spriteCode != KQ_JUMP_END &&
        spriteCode != KQ_AIR_DOUBLE_JUMP &&
        spriteCode != KQ_AIR_NATK) {
        *pXVeclocity = 0;
    }
}

void Keqing::updateAction() {
    // State Changer
    if (isCurrentSprite(KQ_CROUCH)) this->crouch();
    if (isCurrentSprite(KQ_JUMP_START) ||
        isCurrentSprite(KQ_JUMP)) {
        this->jump();
    }
    if (isCurrentSprite(KQ_HURT)) this->damage();

    // Ground
    if (isCurrentSprite(KQ_NATK)) this->NAtk();
    if (isCurrentSprite(KQ_CATK)) this->CAtk();
    if (isCurrentSprite(KQ_DASH)) this->dash();
    if (isCurrentSprite(KQ_SKILL)) this->ESkill();
    if (isCurrentSprite(KQ_SKILL_AIMING)) this->ESkillAiming();
    if (isCurrentSprite(KQ_SKILL_SLASH)) this->ESkillSlash();
    if (isCurrentSprite(KQ_BURST)) this->RBurst();

    // Air
    if (isCurrentSprite(KQ_AIR_DOUBLE_JUMP)) this->airDoubleJump();
    if (isCurrentSprite(KQ_AIR_NATK)) this->airNAtk();
    if (isCurrentSprite(KQ_AIR_PLUNGE)) this->airPlunge();
    if (isCurrentSprite(KQ_AIR_DASH)) this->airDash();
}
