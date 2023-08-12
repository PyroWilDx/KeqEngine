//
// Created by pyrowildx on 13/05/23.
//

#include <cmath>
#include <SDL2/SDL_image.h>
#include "Keqing.hpp"
#include "Entity/Particle.hpp"
#include "Utils/Global.hpp"
#include "Utils/Sound.hpp"

Keqing *Keqing::instance = nullptr;

const int NAtkMax = 4;
const int NAtkEndFrame[NAtkMax] = {3, 9, 17, 28};

Keqing::Keqing()
        : LivingEntity(0.0024, 1, KQ_ENUM_N,
                       KQ_HURT, KQ_JUMP) {
    setHitBox({0, 12, 60, 84});

    this->burstCloneSlashCount = 0;
    this->skillUseTime = 0;
    this->jumpPressTime = 0;
    this->doubleJumped = false;
    this->airDashed = false;
    this->skillAimingSoundChannel = 0;
    this->airPlungeLoopSoundChannel = 0;
    this->locked = false;

    initSprite(KQ_IDLE, "res/gfx/keqing/Idle.png",
               96, 96, 18, 60);
    setXYShift(0, 0, -36, KQ_IDLE);
    setSpriteNext(KQ_IDLE, KQ_IDLE);

    initSprite(KQ_RUN_END, "res/gfx/keqing/RunEnd.png",
               96, 96, 5, 60);
    setXYShift(-16, 0, -16, KQ_RUN_END);

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
    setSpriteNext(KQ_WALK, KQ_WALK_TURN);

    initSprite(KQ_RUN_START, "res/gfx/keqing/RunStart.png",
               96, 96, 3, 60);
    setXYShift(-16, 0, -20, KQ_RUN_START);
    setSpriteNext(KQ_RUN, KQ_RUN_START);

    initSprite(KQ_RUN, "res/gfx/keqing/Run.png",
               128, 96, 8, 60);
    setXYShift(-38, 0, -30, KQ_RUN);
    setSpriteNext(KQ_RUN, KQ_RUN);

    initSprite(KQ_RUN_TURN, "res/gfx/keqing/RunTurn.png",
               96, 96, 3, 30);
    setXYShift(-20, 0, -20, KQ_RUN_TURN);
    setSpriteNext(KQ_RUN, KQ_RUN_TURN);

    initSprite(KQ_CROUCH, "res/gfx/keqing/Crouch.png",
               96, 96, 5, 60);
    setSpriteFrameLengthFromTo(INT32_MAX, 2, 2, KQ_CROUCH);
    setXYShift(-2, 0, -32, KQ_CROUCH);

    initSprite(KQ_JUMP_START, "res/gfx/keqing/JumpStart.png",
               96, 96, 2, 20);
    setXYShift(0, 0, -36, KQ_JUMP_START);
    setSpriteNext(KQ_JUMP, KQ_JUMP_START);

    initSprite(KQ_JUMP, "res/gfx/keqing/Jump.png",
               96, 128, 7, 100);
    setXYShift(0, -34, -34, KQ_JUMP);
    setSpriteFrameLengthFromTo(INT32_MAX, 2, 4, KQ_JUMP);
    setSpriteNext(KQ_JUMP_END, KQ_JUMP);

    initSprite(KQ_NATK, "res/gfx/keqing/NAtk.png",
               192, 160, 34, 60);
    setXYShift(-38, -32, -92, KQ_NATK);

    initSprite(KQ_CATK, "res/gfx/keqing/CAtk.png",
               128, 96, 15, 60);
    setXYShift(-34, 0, -32, KQ_CATK);
    setSpriteFrameLengthFromTo(30, 0, 5, KQ_CATK);

    initSprite(KQ_UP_NATK, "res/gfx/keqing/UpNAtk.png",
               128, 128, 8, 60);
    setXYShift(-22, -32, -46, KQ_UP_NATK);

    initSprite(KQ_UP_CATK, "res/gfx/keqing/UpCAtk.png",
               128, 128, 15, 60);
    setXYShift(-32, -16, -32, KQ_UP_CATK);
    setSpriteFrameLengthFromTo(30, 0, 6, KQ_UP_CATK);

    initSprite(KQ_CROUCH_NATK, "res/gfx/keqing/CrouchNAtk.png",
               160, 64, 6, 60);
    setXYShift(0, 32, -100, KQ_CROUCH_NATK);

    initSprite(KQ_CROUCH_CATK, "res/gfx/keqing/CrouchCAtk.png",
               128, 160, 15, 60);
    setXYShift(-32, -48, -36, KQ_CROUCH_CATK);
    setSpriteFrameLengthFromTo(30, 0, 6, KQ_CROUCH_CATK);

    initSprite(KQ_DASH, "res/gfx/keqing/Dash.png",
               128, 96, 9, 30);
    setXYShift(-24, 0, -42, KQ_DASH);
    setSpriteFrameLengthFromTo(42, 2, 4, KQ_DASH); // Invisible
    setSpriteFrameLengthFromTo(54, 6, -1, KQ_DASH); // End

    initSprite(KQ_SKILL, "res/gfx/keqing/Skill.png",
               128, 96, 13, 60);
    setXYShift(-32, 0, -36, KQ_SKILL);

    initSprite(KQ_SKILL_AIMING, "res/gfx/keqing/SkillAiming.png",
               128, 96, 8, INT32_MAX);
    setXYShift(-32, 0, -36, KQ_SKILL_AIMING);

    initSprite(KQ_SKILL_SLASH, "res/gfx/keqing/SkillSlash.png",
               192, 128, 14, 60);
    setXYShift(-48, -16, -84, KQ_SKILL_SLASH);

    initSprite(KQ_BURST, "res/gfx/keqing/Burst.png",
               128, 96, 30, 70);
    setXYShift(-24, 0, -44, KQ_BURST);
    setSpriteFrameLengthFromTo(INT32_MAX, 13, 13, KQ_BURST);
    setSpriteFrameLengthFromTo(80, 14, 24, KQ_BURST);

    initSprite(KQ_AIR_DOUBLE_JUMP, "res/gfx/keqing/AirDoubleJump.png",
               96, 96, 8, 40);
    setXYShift(-6, 0, -26, KQ_AIR_DOUBLE_JUMP);

    initSprite(KQ_AIR_NATK, "res/gfx/keqing/AirNAtk.png",
               160, 128, 11, 40);
    setXYShift(-48, -8, -48, KQ_AIR_NATK);

    initSprite(KQ_AIR_UP_NATK, "res/gfx/keqing/AirUpNAtk.png",
               94, 160, 7, 60);
    setXYShift(-16, -64, -16, KQ_AIR_UP_NATK);
    setSpriteFrameLengthFromTo(30, 0, 1, KQ_AIR_UP_NATK);

    initSprite(KQ_AIR_SKILL_SLASH, "res/gfx/keqing/AirSkillSlash.png",
               192, 128, 14, 60);
    setXYShift(-40, -16, -84, KQ_AIR_SKILL_SLASH);

    initSprite(KQ_AIR_DASH, "res/gfx/keqing/AirDash.png",
               96, 96, 6, 40);
    setXYShift(-10, 2, -26, KQ_AIR_DASH);

    initSprite(KQ_AIR_PLUNGE, "res/gfx/keqing/AirPlunge.png",
               128, 128, 14, 60);
    setXYShift(-30, -32, -38, KQ_AIR_PLUNGE);

    initSprite(KQ_HURT, "res/gfx/keqing/Hurt.png",
               96, 96, 6, INT32_MAX);
    setXYShift(-4, 0, -32, KQ_HURT);

    setSoundSheet();

    Uint32 rgba = cvStringToUint32(Global::userData[DATA_KQ_COLOR]);
    colorAllSprites(rgba);
}

void Keqing::setSoundSheet() {
    std::string soundStartPath = "res/sfx/keqing/" + Global::userData[DATA_KQ_VOICE_LANG];
    soundSheet->setSoundStartPath(soundStartPath);

    soundSheet->pushSoundPaths(KQ_JUMP_START,
                               "Jump0.ogg",
                               "Jump1.ogg",
                               "Jump2.ogg",
                               "Jump3.ogg",
                               "Jump4.ogg");
    soundSheet->copySoundPaths(KQ_AIR_DOUBLE_JUMP, KQ_JUMP_START);

    soundSheet->pushSoundPaths(KQ_NATK,
                               "AtkWeak0.ogg",
                               "AtkWeak1.ogg",
                               "AtkWeak2.ogg",
                               "AtkWeak3.ogg",
                               "AtkWeak4.ogg",
                               "AtkWeak5.ogg",
                               "AtkWeak6.ogg",
                               "AtkMid0.ogg",
                               "AtkMid1.ogg",
                               "AtkMid2.ogg");
    soundSheet->copySoundPathsStartString(KQ_UP_NATK, KQ_NATK,
                                          "AtkWeak");
    soundSheet->copySoundPathsStartString(KQ_CROUCH_NATK, KQ_NATK,
                                          "AtkWeak");
    soundSheet->copySoundPathsStartString(KQ_AIR_NATK, KQ_NATK,
                                          "AtkWeak");
    soundSheet->copySoundPathsStartString(KQ_AIR_UP_NATK, KQ_NATK,
                                          "AtkWeak");
    soundSheet->copySoundPathsStartString(KQ_AIR_PLUNGE, KQ_NATK,
                                          "AtkMid");

    soundSheet->pushSoundPaths(KQ_CATK,
                               "AtkStrong0.ogg",
                               "AtkStrong1.ogg",
                               "AtkStrong2.ogg");
    soundSheet->copySoundPaths(KQ_UP_CATK, KQ_CATK);
    soundSheet->copySoundPaths(KQ_CROUCH_CATK, KQ_CATK);

    soundSheet->pushSoundPaths(KQ_DASH,
                               "Dash0.ogg",
                               "Dash1.ogg",
                               "Dash2.ogg");
    soundSheet->copySoundPaths(KQ_AIR_DASH, KQ_DASH);

    soundSheet->pushSoundPaths(KQ_SKILL,
                               "Skill0.ogg",
                               "Skill1.ogg",
                               "Skill2.ogg");
    soundSheet->pushSoundPaths(KQ_SKILL_SLASH,
                               "SkillSlash0.ogg",
                               "SkillSlash1.ogg",
                               "SkillSlash2.ogg");
    soundSheet->copySoundPaths(KQ_AIR_SKILL_SLASH, KQ_SKILL_SLASH);

    soundSheet->pushSoundPaths(KQ_BURST,
                               "Burst0.ogg",
                               "Burst1.ogg",
                               "Burst2.ogg");
}

void Keqing::initKeqing() {
    if (instance == nullptr) {
        instance = new Keqing();
    } else {
        SDL_Log("Keqing already initialized!\n");
    }
}

void Keqing::cleanUp() {
    delete instance;
    instance = nullptr;
}

void Keqing::reset() {
    for (int i = 0; i < KQ_ENUM_N; i++) {
        setSpriteAnimated(false, i);
    }
    facingEast = true;
    xVelocity = 0;
    yVelocity = 0;
    skillUseTime = 0;
    burstCloneSlashCount = 0;
    jumpPressTime = 0;
    doubleJumped = false;
    airDashed = false;
}

void Keqing::colorSprite(Uint32 rgba, Sprite *sprite) {
    Uint8 r, g, b;
    Uint32RGBAToUint8RGBA(rgba, &r, &g, &b, nullptr);

    SDL_Renderer *gRenderer = WindowRenderer::getInstance()->getRenderer();

    SDL_Surface *img = IMG_Load(sprite->imgPath);

    auto *pixel0 = (Uint8 *) img->pixels;
    for (Uint8 *pixel = pixel0; pixel < pixel0 + (img->w * img->h) * 4; pixel += 4) {

        double h, s, v;
        RGBToHSV(pixel[0], pixel[1], pixel[2],
                 &h, &s, &v);

        double rgbH, rgbS, rgbV;
        RGBToHSV(r, g, b,
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
            HSVToRGB(h, rgbS, v,
                     &pixel[0], &pixel[1], &pixel[2]);
    }

    SDL_Texture *mTexture = SDL_CreateTextureFromSurface(gRenderer, img);
    SDL_FreeSurface(img);
    SDL_DestroyTexture(sprite->sTexture);
    sprite->sTexture = mTexture;
}

void Keqing::colorCurrSprite(Uint32 rgba) {
    Keqing::colorSprite(rgba, getCurrentSprite());
}

void Keqing::colorAllSprites(Uint32 rgba) {
    std::string valStr = std::to_string(rgba);
    Global::saveUserData(DATA_KQ_COLOR, valStr);

    for (int spriteCode = 0; spriteCode < KQ_ENUM_N; spriteCode++) {
        Keqing::colorSprite(rgba, getSprite(spriteCode));
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

void Keqing::setSpriteAnimated(bool animated, int spriteCode) {
    AnimatedEntity::setSpriteAnimated(animated, spriteCode, &preAction, (void *) this);
}

bool Keqing::shouldUpdateDirection() {
    if (isCurrentSprite(KQ_WALK) ||
        isCurrentSprite(KQ_RUN) ||
        isCurrentSprite(KQ_CROUCH) ||
        isCurrentSprite(KQ_JUMP) ||
        isCurrentSprite(KQ_AIR_DOUBLE_JUMP) ||
        isCurrentSprite(KQ_AIR_NATK)) {
        return true;
    }

    if (isCurrentSprite(KQ_NATK)) {
        for (int endFrameIndex: NAtkEndFrame) {
            if (isFrameBetween(endFrameIndex, endFrameIndex + 1, KQ_NATK)) {
                return true;
            }
        }
    }

    int currSpriteCode = getCurrentSpriteCode();
    if (willFrameFinish(-1, currSpriteCode)) {
        return true;
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
            if (isCurrentSprite(KQ_JUMP) ||
                isCurrentSprite(KQ_AIR_DOUBLE_JUMP) ||
                isCurrentSprite(KQ_AIR_NATK)) {
                xVelocity = KQ_AIR_WALK_VELOCITY;
            } else if (isCurrentSprite(KQ_RUN)) {
                xVelocity = KQ_RUN_VELOCITY;
            } else if (isCurrentSprite(KQ_WALK)) {
                xVelocity = KQ_WALK_VELOCITY;
            }
        } else {
            if (isSpriteAnimated(KQ_RUN)) {
                setSpriteAnimated(false, KQ_RUN);
                setSpriteAnimated(true, KQ_RUN_END);
            } else {
                xVelocity = 0;
                setSpriteAnimated(false, KQ_WALK);
            }
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
        }
        doubleJumped = false;
        airDashed = false;
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

void Keqing::idle() {
    xVelocity = 0;
    yVelocity = 0;
}

void Keqing::walk() {

}

void Keqing::run() {
    if (isSpriteAnimated(KQ_RUN_START)) {
        if (isNewestFrame(0, KQ_RUN_START)) {
            xVelocity = 0.2;

            Sound::playAudioChunk("res/sfx/particle/KQRunStart.ogg");
        }
        xVelocity = std::min(xVelocity + 0.0032 * Global::dt, KQ_RUN_VELOCITY);
    }
    if (isSpriteAnimated(KQ_RUN_END)) {
        if (isNewestFrame(0, KQ_RUN_END)) {
            xVelocity = 0.26;
        }
        xVelocity = std::max(xVelocity - 0.0008 * Global::dt, 0.0);
    }
}

void Keqing::crouch() {
    if (isFrameBetween(0, 2, KQ_CROUCH)) {
        if (!isKeyPressed(KEY_S)) {
            goToFrame(3, KQ_CROUCH);
        }
    }
}

void Keqing::jump() {
    if (isSpriteAnimated(KQ_JUMP_START)) {
        if (isNewestFrame(0, KQ_JUMP_START)) {
            jumpPressTime = Global::pressedTime[KEY_SPACE];
        }

        if (willFrameFinish(-1, KQ_JUMP_START)) {
            yVelocity = -KQ_JUMP_BASE_VELOCITY;

            soundSheet->playRandomSound(KQ_JUMP_START);
            Sound::playAudioChunk("res/sfx/particle/KQJump.ogg");
        }
    }

    if (isSpriteAnimated(KQ_JUMP)) {
        if (isFrameBetween(0, 3, KQ_JUMP)) {
            yVelocity += gravityWeight * (double) Global::dt;
            yVelocity -= gravityWeight * (double) Global::dt; // Cancel Gravity
        }
    }

    if (isSpriteAnimated(KQ_JUMP_END)) {
        xVelocity = 0;
        yVelocity = 0;
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

                soundSheet->playRandomSoundStartString("AtkWeak", KQ_NATK);
            }
        }
    }

    if (isNewestFrame(0, KQ_NATK)) {
        xVelocity = NAtkVelocity[0];

        soundSheet->playRandomSoundStartString("AtkWeak", KQ_NATK);

    } else if (isNewestFrame(30, KQ_NATK)) { // NAtk 4
        soundSheet->playRandomSoundStartString("AtkMid", KQ_NATK);

        Particle *NAtk4Particle =
                Particle::pushParticle(PARTICLE_KQ_NATK_4,
                                       48, 2.4, 2.6);
        NAtk4Particle->setEntity(this);
        NAtk4Particle->xyShift(32, 20);
    }

    // Push Atk
    if (isNewestFrame(1, KQ_NATK)) {
        const int N = 4;
        double atkPolyPts[N][2] =
                {{30,  -12},
                 {110, -18},
                 {16,  20},
                 {30,  0}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(1, 2, KQ_NATK));

    } else if (isNewestFrame(7, KQ_NATK)) {
        const int N = 16;
        double atkPolyPts[N][2] =
                {{16,  -12},
                 {36,  -50},
                 {62,  -46},
                 {86,  -32},
                 {98,  -14},
                 {100, 4},
                 {90,  22},
                 {72,  36},
                 {46,  44},
                 {12,  44},
                 {-16, 34},
                 {-30, 16},
                 {-4,  28},
                 {30,  28},
                 {50,  14},
                 {54,  -12}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(7, 8, KQ_NATK));

    } else if (isNewestFrame(13, KQ_NATK)) {
        const int N = 12;
        double atkPolyPts[N][2] =
                {{-16, -18},
                 {52,  -16},
                 {100, -4},
                 {116, 6},
                 {110, 16},
                 {84,  22},
                 {26,  24},
                 {-42, 12},
                 {-56, 6},
                 {-68, -10},
                 {76,  8},
                 {72,  4}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(13, 14, KQ_NATK));

    } else if (isNewestFrame(21, KQ_NATK)) {
        const int N = 16;
        double atkPolyPts[N][2] =
                {{-16, -48},
                 {-12, -82},
                 {22,  -80},
                 {58,  -64},
                 {76,  -46},
                 {86,  -22},
                 {88,  4},
                 {74,  30},
                 {48,  52},
                 {14,  62},
                 {-20, 58},
                 {-48, 38},
                 {-6,  52},
                 {34,  42},
                 {54,  6},
                 {32,  -34}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(21, 22, KQ_NATK));

    } else if (isNewestFrame(24, KQ_NATK)) {
        const int N = 20;
        double atkPolyPts[N][2] =
                {{20,  -28},
                 {66,  -52},
                 {86,  -50},
                 {114, -42},
                 {124, -28},
                 {124, -14},
                 {114, 2},
                 {82,  16},
                 {46,  24},
                 {-24, 26},
                 {-60, 18},
                 {-68, 10},
                 {-68, 2},
                 {-48, -10},
                 {-58, -2},
                 {-58, 8},
                 {-24, 16},
                 {52,  6},
                 {74,  -8},
                 {70,  -22}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(24, 25, KQ_NATK));

    } else if (isNewestFrame(30, KQ_NATK)) {
        const int N = 6;
        double atkPolyPts[N][2] =
                {{8,   16},
                 {84,  2},
                 {114, 10},
                 {126, 16},
                 {114, 22},
                 {84,  30}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 2.6, -0.1);
        atk->setAtkDuration(getSpriteLengthFromTo(30, 32, KQ_NATK));
    }
}

void Keqing::CAtk() {
    if (isNewestFrame(6, KQ_CATK)) {
        xVelocity = -0.4;

        soundSheet->playRandomSound(KQ_CATK);

        Particle *CAtkParticle =
                Particle::pushParticle(PARTICLE_KQ_CATK, 60);
        CAtkParticle->moveToEntityCenter(this);
        CAtkParticle->xyShift(30, 10);

        Particle *skillIdleParticle =
                Particle::getParticle(PARTICLE_KQ_SKILL_IDLE);
        if (skillIdleParticle != nullptr) {
            Particle::removeParticle(PARTICLE_HUD_SKILL_ICON_2);
        }
    }

    if (xVelocity < 0) {
        xVelocity = std::min(xVelocity + 0.001 * Global::dt, 0.0);
    }
}

void Keqing::upNAtk() {
    if (isNewestFrame(0, KQ_UP_NATK)) {
        soundSheet->playRandomSound(KQ_UP_NATK);
    }

    if (isFrameBetween(6, -1, KQ_UP_NATK)) {
        if (isKeyPressed(KEY_Z) && isKeyPressedLong(KEY_MOUSE_LEFT)) {
            setSpriteAnimated(false, KQ_UP_NATK);
            setSpriteAnimated(true, KQ_UP_CATK);
        }
    }
}

void Keqing::upCAtk() {
    if (isNewestFrame(7, KQ_UP_CATK)) {
        soundSheet->playRandomSound(KQ_UP_CATK);

        Particle *upCAtkPartcle =
                Particle::pushParticle(PARTICLE_KQ_UP_CATK, 40);
        upCAtkPartcle->moveToEntityCenter(this);
        upCAtkPartcle->xyShift(0, -24);
    }
}

void Keqing::crouchNAtk() {
    if (isNewestFrame(0, KQ_CROUCH_NATK)) {
        soundSheet->playRandomSound(KQ_CROUCH_NATK);
    }

    if (isFrameBetween(4, -1, KQ_CROUCH_NATK)) {
        if (isKeyPressed(KEY_S) && isKeyPressedLong(KEY_MOUSE_LEFT)) {
            setSpriteAnimated(false, KQ_CROUCH_NATK);
            setSpriteAnimated(true, KQ_CROUCH_CATK);
        }
    }
}

void Keqing::crouchCAtk() {
    if (isNewestFrame(7, KQ_CROUCH_CATK)) {
        soundSheet->playRandomSound(KQ_CROUCH_CATK);

        Particle *crouchCAtkPartcle =
                Particle::pushParticle(PARTICLE_KQ_CROUCH_CATK, 40);
        crouchCAtkPartcle->moveToEntityCenter(this);
        crouchCAtkPartcle->xyShift(0, 24);
    }
}

void Keqing::dash() {
    if (isNewestFrame(0, KQ_DASH)) {
        if (isKeyPressed(KEY_Q)) setFacingEast(true);
        else if (isKeyPressed(KEY_D)) setFacingEast(false);

        soundSheet->playRandomSound(KQ_DASH);

    } else if (isNewestFrame(2, KQ_DASH)) {
        xVelocity = -KQ_DASH_VELOCITY;

    } else if (isFrameBetween(6, -1, KQ_DASH)) {
        if (xVelocity < 0) {
            xVelocity = std::min(xVelocity + 0.01 * (double) Global::dt, 0.0);
        }
    }
}

void Keqing::createLightningStelitto(int mouseX, int mouseY) {
    Particle *skillCircleHud = Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE);
    Particle *skillIcon2 =
            Particle::pushParticle(PARTICLE_HUD_SKILL_ICON_2,
                                   KQ_LIGHTNING_STILETTO_DURATION,
                                   HUB_SB_ICON_M, HUB_SB_ICON_M);
    skillIcon2->moveToEntityCenter(skillCircleHud);
    skillIcon2->setOnRemove([](Particle *removedParticle) {
        Particle *skillCircleHud = Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE);

        Particle *skillIcon1 =
                Particle::pushParticle(PARTICLE_HUD_SKILL_ICON_1,
                                       INT32_MAX,
                                       HUB_SB_ICON_M, HUB_SB_ICON_M);
        skillIcon1->moveToEntityCenter(skillCircleHud);
        skillIcon1->setRGBAMod(COLOR_WHITE, HUD_SB_USED_ALPHA);

        int elapsedTime = getTime() - Keqing::getInstance()->getSkillUseTime();
        Particle *timerHud =
                Particle::pushParticle(PARTICLE_HUD_SKILL_BURST_TIMER,
                                       (KQ_SKILL_COOLDOWN - elapsedTime) / HUD_SB_TIMER_FRAME_N,
                                       HUD_SB_CIRCLE_M * 1.0,
                                       HUD_SB_CIRCLE_M * 1.0);
        timerHud->moveToEntityCenter(skillCircleHud);
        timerHud->setOnRemove([](Particle *removedParticle) {
            Particle *skillCircleHud =
                    Particle::pushParticle(PARTICLE_HUD_SKILL_CIRCLE, INT32_MAX,
                                           HUD_SB_CIRCLE_M, HUD_SB_CIRCLE_M);
            skillCircleHud->moveToEntityCenter(
                    Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE_BG));
            skillCircleHud->setRGBAMod(KQ_SKILL_CIRCLE_RGBA);
            Particle *skillIcon1Hud = Particle::getParticle(PARTICLE_HUD_SKILL_ICON_1);
            skillIcon1Hud->setRGBAMod(COLOR_WHITE_FULL);
        });

        Particle::removeParticle(PARTICLE_HUD_SKILL_CIRCLE);

        Particle::removeParticle(PARTICLE_KQ_SKILL_IDLE);
    });

    Particle::removeParticle(PARTICLE_HUD_SKILL_ICON_1);
    skillCircleHud->setRGBAMod(238, 10, 238, 255);

    Particle *spawnParticle =
            Particle::pushParticle(PARTICLE_KQ_SKILL_SPAWN, 60);
    if (mouseX == -1 || mouseY == -1) {
        spawnParticle->moveToEntityCenter(this);
        double distance = SKILL_TP_DISTANCE;
        if (!isFacingEast()) distance = -distance;
        spawnParticle->moveAdd(distance, 0);
    } else {
        spawnParticle->moveToCenter(mouseX, mouseY);
    }

    spawnParticle->setHitBoxCentered((int) getBaseHitBoxW(),
                                     (int) getBaseHitBoxH());
    spawnParticle->checkXCollision(x < spawnParticle->getX());
    spawnParticle->checkYCollision(true);
    spawnParticle->delaySprite(INT32_MAX);

    spawnParticle->setOnRender([](Particle *particle) {
        if (particle->isNewestFrame(6)) {
            Particle *idleParticle =
                    Particle::pushParticle(PARTICLE_KQ_SKILL_IDLE,
                                           200, 2.0, 2.0);
            idleParticle->moveToEntityCenter(particle);
            idleParticle->setHitBoxCentered(particle->getHitBox().w,
                                            particle->getHitBox().h);
            idleParticle->setOnRender([](Particle *particle) {
                SDL_Rect dstParticleRect = particle->getRenderRect();
                SDL_Renderer *gRenderer = WindowRenderer::getInstance()->getRenderer();
                const int rectH = 6;
                SDL_Rect dstRect = {dstParticleRect.x,
                                    dstParticleRect.y + dstParticleRect.h + IDLE_PARTICLE_GAP - rectH,
                                    dstParticleRect.w,
                                    rectH - 2};
                WindowRenderer::renderRect(&dstRect, true,
                                           COLOR_BLACK_FULL,
                                           gRenderer,
                                           false, false);
            });
        }
    });

    Particle *projParticle =
            Particle::pushParticle(PARTICLE_KQ_SKILL_PROJ, INT32_MAX);
    double kqCenterX, kqCenterY;
    getSelfCenter(&kqCenterX, &kqCenterY);

    double *spawnParticleCenterX = new double;
    double *spawnParticleCenterY = new double;
    spawnParticle->getSelfCenter(spawnParticleCenterX, spawnParticleCenterY);

    double xDiff = *spawnParticleCenterX - kqCenterX;
    if (!isFacingEast()) xDiff = -xDiff;
    double yDiff = *spawnParticleCenterY - kqCenterY;

    double angle = atan2Pos(yDiff, xDiff);
    projParticle->setRotation(radToDegree(angle));

    projParticle->moveToEntityCenter(this);

    f2Params *fParams = new f2Params;
    fParams->fParam0 = (void *) spawnParticleCenterX;
    fParams->fParam1 = (void *) spawnParticleCenterY;

    projParticle->setOnRender([](Particle *particle) {
        double lastX = particle->getX();
        double lastY = particle->getY();

        double angleDeg = particle->getRotation();
        double angle = degreeToRad(angleDeg);

        Particle *projAftFxParticle =
                Particle::pushParticle(PARTICLE_KQ_SKILL_PROJ_AFT_FX, 20);
        projAftFxParticle->setRotation(angleDeg);
        projAftFxParticle->moveToEntityCenter(particle);

        double distance = LIGHTNING_STELITTO_VELOCITY * (double) Global::dt;
        double addX = cos(angle) * distance;
        if (!particle->isFacingEast()) addX = -addX;
        double addY = sin(angle) * distance;
        particle->moveAdd(addX, addY);

        f2Params *fParams = (f2Params *) particle->getOnRenderParams();
        double dstX = *((double *) fParams->fParam0);
        double dstY = *((double *) fParams->fParam1);
        if ((lastX < dstX && particle->getX() >= dstX) ||
            (lastX > dstX && particle->getX() <= dstX) ||
            (lastY < dstY && particle->getY() >= dstY) ||
            (lastY > dstY && particle->getY() <= dstY)) {
            Particle::removeParticle(PARTICLE_KQ_SKILL_PROJ);
            
            Particle *spawnParticle =
                    Particle::getParticle(PARTICLE_KQ_SKILL_SPAWN);
            spawnParticle->resetSprite();

            Sound::playAudioChunk("res/sfx/particle/KQSkillShoot.ogg");
        }
    }, fParams);

    projParticle->setOnRemove([](Particle *particle) {
        f2Params *fParams = (f2Params *) particle->getOnRenderParams();
        delete ((double *) fParams->fParam0);
        delete ((double *) fParams->fParam1);
        delete fParams;
    });

    skillUseTime = Global::currentTime;

    Sound::deleteAudioChunk(skillAimingSoundChannel);
    soundSheet->playRandomSound(KQ_SKILL);
    Sound::playAudioChunk("res/sfx/particle/KQSkillShoot.ogg");
}

void Keqing::ESkill() {
    if (isNewestFrame(0, KQ_SKILL)) {
        Sound::playAudioChunk("res/sfx/particle/KQSkillStart.ogg");
    }

    if (willFrameFinish(3, KQ_SKILL)) {
        if (!isKeyPressedLong(KEY_E)) {
            createLightningStelitto();
        } else {
            pauseSprite(true, KQ_SKILL);
            setSpriteAnimated(true, KQ_SKILL_AIMING);
        }
    }
}

void Keqing::ESkillAiming() {
    Particle *cursorIdleParticle =
            Particle::getParticle(PARTICLE_KQ_SKILL_AIMING_IDLE);
    if (cursorIdleParticle == nullptr) {
        cursorIdleParticle =
                Particle::pushParticle(PARTICLE_KQ_SKILL_AIMING_IDLE,
                                       60, 2.22, 2.22);

        skillAimingSoundChannel = Sound::playAudioChunk(
                "res/sfx/particle/KQSkillAiming.ogg", INT32_MAX);
    }
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    getMouseAbsoluteXYNotTranslated(&mouseX, &mouseY);

    double kqCenterX, kqCenterY;
    getSelfCenter(&kqCenterX, &kqCenterY);
    double xDiff = (double) mouseX - kqCenterX;
    if (!isFacingEast()) xDiff = -xDiff;
    double yDiff = kqCenterY - (double) mouseY;
    double angle = atan2Pos(yDiff, xDiff);

    double dist = getDistance(kqCenterX, kqCenterY,
                              (double) mouseX, (double) mouseY);
    if (dist > SKILL_TP_DISTANCE) {
        double absX = cos(angle) * SKILL_TP_DISTANCE;
        if (!isFacingEast()) absX = -absX;
        mouseX = (int) (kqCenterX + absX);
        mouseY = (int) (kqCenterY - sin(angle) * SKILL_TP_DISTANCE);
    }

    int frameIndex = (int) ((angle / (2.0 * M_PI)) * 8);
    goToFrame(frameIndex, KQ_SKILL_AIMING);

    cursorIdleParticle->moveToCenter((double) mouseX, (double) mouseY);

    if (!isKeyPressed(KEY_E)) {
        Particle::removeParticle(PARTICLE_KQ_SKILL_AIMING_IDLE);
        setSpriteAnimated(false, KQ_SKILL_AIMING);
        createLightningStelitto(mouseX, mouseY);
        pauseSprite(false, KQ_SKILL);
        goToFrame(5, KQ_SKILL);
        return;
    }
}

void Keqing::doSkillSlashFrame0() {
    Particle *idleParticle =
            Particle::getParticle(PARTICLE_KQ_SKILL_IDLE);
    if (x <= idleParticle->getX()) setFacingEast(true);
    else setFacingEast(false);
}

void Keqing::moveToLStiletto() {
    Particle *idleParticle =
            Particle::getParticle(PARTICLE_KQ_SKILL_IDLE);
    if (idleParticle != nullptr) {
        this->moveToEntityCenter(idleParticle, false);
        double idleParticleHeight;
        idleParticle->getRealSize(nullptr, &idleParticleHeight);
        y = idleParticle->getY() - (hitBox.h - idleParticleHeight) - hitBox.y - 2 + IDLE_PARTICLE_GAP;

        Sound::playAudioChunk("res/sfx/particle/KQSkillTP.ogg");
    } else {
        SDL_Log("Lightning Stiletto doesn't exists!\n");
    }
}

void Keqing::doSkillTP() {
    Particle *tpStartParticle =
            Particle::pushParticle(PARTICLE_KQ_BURST_VANISH, 60);
    tpStartParticle->moveToEntityCenter(this);

    moveToLStiletto();

    Particle *tpEndParticle =
            Particle::pushParticle(PARTICLE_KQ_SKILL_TP_END,
                                   60, 0.64, 0.64);
    tpEndParticle->moveToEntityCenter(this);

    Particle::removeParticle(PARTICLE_HUD_SKILL_ICON_2);
}

void Keqing::ESkillSlash() {
    yVelocity = 0;

    if (isNewestFrame(0, KQ_SKILL_SLASH)) {
        doSkillSlashFrame0();

    } else if (isNewestFrame(6, KQ_SKILL_SLASH)) {
        Particle *idleParticle =
                Particle::getParticle(PARTICLE_KQ_SKILL_IDLE);
        bool isIdleParticleInAir = idleParticle->isInAir();

        doSkillTP();

        soundSheet->playRandomSound(KQ_SKILL_SLASH);

        if (isIdleParticleInAir) {
            setSpriteAnimated(false, KQ_SKILL_SLASH);
            setSpriteAnimated(true, KQ_AIR_SKILL_SLASH);
            goToFrameNoNew(6, KQ_AIR_SKILL_SLASH);
        }
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

void pushCloneSlashParticle(Particle *removedParticle) {
    Keqing *kq = Keqing::getInstance();

    if (removedParticle == nullptr) {
        kq->setBurstCloneSlashCount(0);
    }

    int cSlashCount = kq->getBurstCloneSlashCount();
    if (cSlashCount < KQ_BURST_NUMBER_OF_CLONE_SLASH) {
        auto *cSlashParticle =
                Particle::pushParticle(PARTICLE_KQ_BURST_CLONE_SLASH,
                                       cSlashFrameDuration,
                                       cSlashWM[cSlashCount],
                                       cSlashHM[cSlashCount]);
        cSlashParticle->moveToEntityCenter(Keqing::getInstance());
        cSlashParticle->xyShift(cSlashXShift[cSlashCount], cSlashYShift[cSlashCount]);
        cSlashParticle->setRotation(cSlashRotations[cSlashCount]);
        cSlashParticle->setOnRemove(&pushCloneSlashParticle);

        kq->setBurstCloneSlashCount(cSlashCount + 1);
    }
}

const int slashDuration = 46;
const int slashRotations[KQ_BURST_NUMBER_OF_SLASH] =
        {0, 20, 40, 60, 80, 100, 120, 140};

const int cVanishDuration = 40;
const double cVanishXShift[KQ_BURST_NUMBER_OF_CLONE] =
        {100, -80, 74, -100, -14};
const double cVanishYShift[KQ_BURST_NUMBER_OF_CLONE] =
        {26, -36, -48, 36, -80};

const double aoeBaseWHM = 0;
const double aoeMaxWHM = 2.0;
const int cAppearDuration = 48;
const double cAppearXShift[KQ_BURST_NUMBER_OF_CLONE] =
        {142, -120, 98, -136, -16};
const double cAppearYShift[KQ_BURST_NUMBER_OF_CLONE] =
        {42, -40, -58, 58, -110};

void Keqing::RBurst() {
    if (isNewestFrame(0, KQ_BURST)) { // Burst Start
        Particle::removeParticle(PARTICLE_HUD_BURST_CIRCLE);
        Particle *burstIconHud =
                Particle::getParticle(PARTICLE_HUD_BURST_ICON);
        burstIconHud->setRGBAMod(COLOR_WHITE, HUD_SB_USED_ALPHA);

        Particle::pushParticle(PARTICLE_KQ_BURST_AOE,
                               80, aoeBaseWHM, aoeBaseWHM);

        Particle *aoeWaveParticle =
                Particle::pushParticle(PARTICLE_KQ_BURST_AOE_WAVE,
                                       60, aoeBaseWHM, aoeBaseWHM);
        aoeWaveParticle->setRGBAMod(COLOR_WHITE, 128);

        soundSheet->playRandomSound(KQ_BURST);
        Sound::playAudioChunk("res/sfx/particle/KQBurstStart.ogg");

    } else if (isNewestFrame(12, KQ_BURST)) { // Vanish
        Particle *vanishParticle =
                Particle::pushParticle(PARTICLE_KQ_BURST_VANISH, 60);
        vanishParticle->setEntity(this);

    } else if (isNewestFrame(13, KQ_BURST)) { // Clone Slash Start
        pushCloneSlashParticle(nullptr);
        Particle *cloneParticle =
                Particle::pushParticle(PARTICLE_KQ_BURST_CLONE,
                                       (int) (cSlashFrameDuration * 1.8),
                                       2.0, 2.0);
        cloneParticle->moveToEntityCenter(this);
        cloneParticle->xyShift(0, -46);
        cloneParticle->delaySprite(cSlashFrameDuration);
        cloneParticle->stopOnFrame(-1);

        Sound::playAudioChunk("res/sfx/particle/KQBurstRapid.ogg");

    } else if (isNewestFrame(26, KQ_BURST)) { // Final Slash
        Particle *aoeParticle =
                Particle::getParticle(PARTICLE_KQ_BURST_AOE);
        Particle *aoeWaveParticle =
                Particle::getParticle(PARTICLE_KQ_BURST_AOE_WAVE);

        Particle *finalSlashParticle =
                Particle::pushParticle(PARTICLE_KQ_BURST_FINAL_SLASH, 80);
        finalSlashParticle->moveToEntityCenter(aoeParticle);

        Particle *burstCloneParticle =
                Particle::getParticle(PARTICLE_KQ_BURST_CLONE);
        for (int i = 0; i < KQ_BURST_NUMBER_OF_CLONE; i++) {
            Particle *cloneVanishParticle =
                    Particle::pushParticle(PARTICLE_KQ_BURST_CLONE_VANISH,
                                           cVanishDuration,
                                           1.2, 1.2);
            cloneVanishParticle->moveToEntityCenter(burstCloneParticle);
            cloneVanishParticle->xyShift(cVanishXShift[i], cVanishYShift[i]);
        }

        aoeParticle->fadeAway();
        aoeWaveParticle->fadeAway();

        Particle::removeParticle(PARTICLE_KQ_BURST_CLONE);

        Sound::playAudioChunk("res/sfx/particle/KQBurstEnd.ogg");
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
                        Particle::pushParticle(PARTICLE_KQ_BURST_CLONE_APPEAR,
                                               cAppearDuration);
                cloneAppearParticle->moveToEntityCenter(this);
                cloneAppearParticle->xyShift(cAppearXShift[i], cAppearYShift[i]);
            }
        }

        if (!Particle::isActive(PARTICLE_KQ_BURST_CLONE_SLASH)) { // Clone Slash End
            goToNextFrame(KQ_BURST);

            for (int i = 0; i < KQ_BURST_NUMBER_OF_SLASH; i++) {
                Particle *slashParticle =
                        Particle::pushParticle(PARTICLE_KQ_BURST_SLASH,
                                               slashDuration,
                                               1.2, 1.2);
                slashParticle->moveToEntityCenter(aoeParticle);
                slashParticle->setRotation(slashRotations[i]);
                slashParticle->delaySprite(2 * slashDuration * i);
            }
        }
    }
}

void Keqing::airDoubleJump() {
    if (isNewestFrame(0, KQ_AIR_DOUBLE_JUMP)) {
        doubleJumped = true;
        yVelocity = -KQ_AIR_DOUBLE_JUMP_BASE_VELOCITY;

        soundSheet->playRandomSound(KQ_AIR_DOUBLE_JUMP);
    }
}

void Keqing::airNAtk() {
    if (yVelocity == 0) {
        setSpriteAnimated(false, KQ_AIR_NATK);
        return;
    }

    if (isNewestFrame(0, KQ_AIR_NATK)) {
        soundSheet->playRandomSound(KQ_AIR_NATK);
    }

    // Push Atk
    if (isNewestFrame(2, KQ_AIR_NATK)) {
        const int N = 10;
        double atkPolyPts[N][2] =
                {{28,  30},
                 {-32, 30},
                 {-54, 26},
                 {-72, 12},
                 {-76, -6},
                 {-42, -14},
                 {-26, -8},
                 {-44, 0},
                 {-40, 12},
                 {-32, 20}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(2, 2, KQ_AIR_NATK));

    } else if (isNewestFrame(3, KQ_AIR_NATK)) {
        const int N = 12;
        double atkPolyPts[N][2] =
                {{-42, 24},
                 {-58, 20},
                 {-70, 12},
                 {-76, 2},
                 {-72, -6},
                 {-62, -18},
                 {-32, -26},
                 {6,   -28},
                 {-10, -12},
                 {-58, -2},
                 {-62, 6},
                 {-60, 14}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(3, 3, KQ_AIR_NATK));

    } else if (isNewestFrame(4, KQ_AIR_NATK)) {
        const int N = 10;
        double atkPolyPts[N][2] =
                {{-64, -14},
                 {-46, -24},
                 {-16, -30},
                 {40,  -28},
                 {62,  -22},
                 {74,  -14},
                 {84,  -2},
                 {42,  8},
                 {40,  -10},
                 {22,  -20}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(4, 4, KQ_AIR_NATK));

    } else if (isNewestFrame(5, KQ_AIR_NATK)) {
        const int N = 14;
        double atkPolyPts[N][2] =
                {{0,  -28},
                 {26, -28},
                 {50, -24},
                 {72, -14},
                 {84, 0},
                 {78, 10},
                 {62, 22},
                 {34, 28},
                 {0,  18},
                 {10, 8},
                 {30, 12},
                 {56, 0},
                 {60, 8},
                 {48, -16}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(5, 5, KQ_AIR_NATK));

    } else if (isNewestFrame(6, KQ_AIR_NATK)) {
        const int N = 20;
        double atkPolyPts[N][2] =
                {{68,  -16},
                 {78,  -8},
                 {82,  6},
                 {70,  18},
                 {46,  26},
                 {-12, 28},
                 {-46, 22},
                 {-70, 10},
                 {-78, 0},
                 {-76, -12},
                 {-66, -24},
                 {-44, -30},
                 {-30, -32},
                 {-8,  -6},
                 {-32, -8},
                 {-38, 0},
                 {-24, 14},
                 {16,  16},
                 {52,  12},
                 {70,  2}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(6, 7, KQ_AIR_NATK));
    }
}

void Keqing::airUpNAtk() {
    if (yVelocity == 0) {
        setSpriteAnimated(false, KQ_AIR_UP_NATK);
        return;
    }

    if (isNewestFrame(0, KQ_AIR_UP_NATK)) {
        soundSheet->playRandomSound(KQ_AIR_UP_NATK);
    }
}

void Keqing::airESkillSlash() {
    yVelocity = 0;

    if (isNewestFrame(0, KQ_AIR_SKILL_SLASH)) {
        doSkillSlashFrame0();

    } else if (isNewestFrame(6, KQ_AIR_SKILL_SLASH)) {
        Particle *idleParticle =
                Particle::getParticle(PARTICLE_KQ_SKILL_IDLE);
        bool isIdleParticleInAir = idleParticle->isInAir();

        doSkillTP();

        soundSheet->playRandomSound(KQ_AIR_SKILL_SLASH);

        if (!isIdleParticleInAir) {
            setSpriteAnimated(false, KQ_AIR_SKILL_SLASH);
            setSpriteAnimated(true, KQ_SKILL_SLASH);
            goToFrameNoNew(6, KQ_SKILL_SLASH);
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

        soundSheet->playRandomSound(KQ_AIR_DASH);
    }
}

void Keqing::airPlunge() { // Plunge Attack in Genshin
    if (isNewestFrame(0, KQ_AIR_PLUNGE)) {
        setSpriteAnimated(false, KQ_JUMP);

        soundSheet->playRandomSound(KQ_AIR_PLUNGE);
        Sound::playAudioChunk("res/sfx/particle/KQPlungeStart.ogg");

    } else if (isNewestFrame(1, KQ_AIR_PLUNGE)) {
        yVelocity = -0.4;

    } else if (isNewestFrame(6, KQ_AIR_PLUNGE)) {
        Particle *airNAttackParticle =
                Particle::pushParticle(PARTICLE_KQ_AIR_PLUNGE,
                                       60, 3.2, 0.1);
        airNAttackParticle->setEntity(this);
        airNAttackParticle->xyShift(26, 52);

        yVelocity = 0.2;

        airPlungeLoopSoundChannel = Sound::playAudioChunk(
                "res/sfx/particle/KQPlungeLoop.ogg", INT32_MAX);
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
            goToFrameNoNew(6, KQ_AIR_PLUNGE);
        }
    }

    if (isFrameBetween(6, 7, KQ_AIR_PLUNGE)) {
        if (yVelocity == 0) {
            goToFrame(8, KQ_AIR_PLUNGE);
            Particle::removeParticle(PARTICLE_KQ_AIR_PLUNGE);
            Particle *airNAtkGroundParticle =
                    Particle::pushParticle(PARTICLE_KQ_AIR_PLUNGE_GROUND, 60);
            airNAtkGroundParticle->moveToEntityCenter(this);
            airNAtkGroundParticle->xyShift(20, -6);

            Sound::deleteAudioChunk(airPlungeLoopSoundChannel);
            Sound::playAudioChunk("res/sfx/particle/KQPlungeEnd.ogg");
        }
    }

    // Push Atk
    if (isNewestFrame(6, KQ_AIR_PLUNGE)) {
        const int N = 6;
        double atkPolyPts[N][2] =
                {{28, 42},
                 {18, 34},
                 {20, -16},
                 {28, -20},
                 {36, -16},
                 {38, 34}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setShouldRemove([](Attack *self, void *fParams) {
            return (Particle::getParticle(PARTICLE_KQ_AIR_PLUNGE) == nullptr);
        }, nullptr);
    }
}

void Keqing::onGameFrame() {
    if (!locked) {
        LivingEntity::onGameFrame();

        updateActionFromKey();

        fallGravity();

        if (shouldUpdateDirection()) updateDirection();
        if (canMoveLR()) moveLR();

        updateAction();

        moveX();
        moveY();

        airAnimate();
    }

    animateSprite();
}

void Keqing::hurt() {
    LivingEntity::hurt();
}

void Keqing::setFacingEast(bool value) {
    if (facingEast != value) {
        if (!isInAir()) {
            if (!isSpriteAnimated(KQ_RUN_START) &&
                !isSpriteAnimated(KQ_RUN) &&
                !isSpriteAnimated(KQ_RUN_TURN)) {
                setSpriteAnimated(true, KQ_WALK_TURN);
            } else {
                setSpriteAnimated(true, KQ_RUN_TURN);
            }
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
            spriteCodeToSkip[KQ_AIR_UP_NATK] = true;
            break;

        case KQ_AIR_DASH:
            if (airDashed) return false;
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
    auto *kq = (Keqing *) fParams;
    if (spriteCode != KQ_WALK &&
        spriteCode != KQ_RUN_START &&
        spriteCode != KQ_RUN &&
        spriteCode != KQ_RUN_END &&
        spriteCode != KQ_JUMP_START &&
        spriteCode != KQ_JUMP &&
        spriteCode != KQ_JUMP_END &&
        spriteCode != KQ_AIR_DOUBLE_JUMP &&
        spriteCode != KQ_AIR_NATK) {
        kq->xVelocity = 0;
    }
}

void Keqing::updateActionFromKey() {
    int spriteCode = KQ_IDLE;

    if (isKeyPressed(KEY_R)) {
        if (!isInAir()) {
            spriteCode = KQ_BURST;
        }

    } else if (isKeyPressed(KEY_E)) {
        if (!Particle::isActive(PARTICLE_KQ_SKILL_IDLE)) {
            if (!isInAir()) {
                spriteCode = KQ_SKILL;
            }
        } else {
            if (isInAir()) {
                spriteCode = KQ_AIR_SKILL_SLASH;
            } else {
                spriteCode = KQ_SKILL_SLASH;
            }
        }

    } else if (isKeyPressed(KEY_SHIFT)) {
        if (isInAir()) {
            spriteCode = KQ_AIR_DASH;
        } else {
            if (isKeyPressed(KEY_Q) || isKeyPressed(KEY_D)) {
                if (isKeyPressedShort(KEY_SHIFT)) {
                    spriteCode = KQ_DASH;
                }
            }
        }

    } else if (isKeyPressed(KEY_MOUSE_LEFT)) {
        if (isInAir()) {
            if (isKeyPressed(KEY_S)) {
                spriteCode = KQ_AIR_PLUNGE;
            } else if (isKeyPressed(KEY_Z)) {
                spriteCode = KQ_AIR_UP_NATK;
            } else {
                spriteCode = KQ_AIR_NATK;
            }
        } else {
            if (isKeyPressed(KEY_Z)) {
                spriteCode = KQ_UP_NATK;
            } else if (isKeyPressed(KEY_S)) {
                spriteCode = KQ_CROUCH_NATK;
            } else {
                spriteCode = KQ_NATK;
            }
        }

    } else if (isKeyPressed(KEY_SPACE)) {
        if (isInAir()) {
            spriteCode = KQ_AIR_DOUBLE_JUMP;
        } else {
            spriteCode = KQ_JUMP_START;
        }

    } else if (isKeyPressed(KEY_S)) {
        if (!isInAir()) {
            spriteCode = KQ_CROUCH;
        }

    } else if (isKeyPressed(KEY_Q) ||
               isKeyPressed(KEY_D)) {
        if (!isKeyPressed(KEY_MOUSE_RIGHT)) {
            spriteCode = KQ_WALK;
        } else {
            if (!isSpriteAnimated(KQ_RUN_END)) {
                spriteCode = KQ_RUN_START;
            } else {
                spriteCode = KQ_RUN;
            }
        }

    }

    if (canDoAction(spriteCode)) setSpriteAnimated(true, spriteCode);
}

void Keqing::updateAction() {
    LivingEntity::updateAction();

    // State Changer
    if (isSpriteAnimated(KQ_IDLE)) this->idle();
    if (isSpriteAnimated(KQ_WALK)) this->walk();
    if (isSpriteAnimated(KQ_RUN_START) ||
        isSpriteAnimated(KQ_RUN) ||
        isSpriteAnimated(KQ_RUN_TURN) ||
        isSpriteAnimated(KQ_RUN_END)) {
        this->run();
    }
    if (isSpriteAnimated(KQ_CROUCH)) this->crouch();
    if (isSpriteAnimated(KQ_JUMP_START) ||
        isSpriteAnimated(KQ_JUMP) ||
        isSpriteAnimated(KQ_JUMP_END)) {
        this->jump();
    }
    if (isSpriteAnimated(KQ_HURT)) this->hurt();

    // Ground
    if (isSpriteAnimated(KQ_NATK)) this->NAtk();
    if (isSpriteAnimated(KQ_CATK)) this->CAtk();
    if (isSpriteAnimated(KQ_UP_NATK)) this->upNAtk();
    if (isSpriteAnimated(KQ_UP_CATK)) this->upCAtk();
    if (isSpriteAnimated(KQ_DASH)) this->dash();
    if (isSpriteAnimated(KQ_SKILL)) this->ESkill();
    if (isSpriteAnimated(KQ_SKILL_AIMING)) this->ESkillAiming();
    if (isSpriteAnimated(KQ_SKILL_SLASH)) this->ESkillSlash();
    if (isSpriteAnimated(KQ_BURST)) this->RBurst();

    // Crouch
    if (isSpriteAnimated(KQ_CROUCH_NATK)) this->crouchNAtk();
    if (isSpriteAnimated(KQ_CROUCH_CATK)) this->crouchCAtk();

    // Air
    if (isSpriteAnimated(KQ_AIR_DOUBLE_JUMP)) this->airDoubleJump();
    if (isSpriteAnimated(KQ_AIR_NATK)) this->airNAtk();
    if (isSpriteAnimated(KQ_AIR_UP_NATK)) this->airUpNAtk();
    if (isSpriteAnimated(KQ_AIR_SKILL_SLASH)) this->airESkillSlash();
    if (isSpriteAnimated(KQ_AIR_DASH)) this->airDash();
    if (isSpriteAnimated(KQ_AIR_PLUNGE)) this->airPlunge();
}

void Keqing::lock() {
    locked = true;
    for (int i = KQ_IDLE + 1; i < KQ_ENUM_N; i++) {
        setSpriteAnimated(false, i);
    }
    setSpriteAnimated(true, KQ_IDLE);
}

void Keqing::unlock() {
    locked = false;
}
