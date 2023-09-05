//
// Created by pyrowildx on 13/05/23.
//

#include <cmath>
#include <SDL2/SDL_image.h>
#include "Keqing.hpp"
#include "Entity/Particle.hpp"
#include "Utils/Global.hpp"
#include "Utils/Sound.hpp"
#include "Utils/Random.hpp"

Keqing *Keqing::instance = nullptr;

const int NAtkMax = 4;
const int NAtkEndFrame[NAtkMax] = {3, 9, 17, 28};

Keqing::Keqing()
        : LivingEntity(0.0024, 1, KQ_ENUM_N,
                       KQ_HURT, KQ_JUMP) {
    setHitBox({0, 12, 60, 84});

    this->jumpPressTime = 0;
    this->ASkillFlipPressTime = 0;
    this->ASkillCloneCenterX = 0;
    this->ASkillCloneCenterY = 0;
    this->ESkillPausedSpriteCode = 0;
    this->ESkillCursorSoundChannel = 0;
    this->ESkillX = 0;
    this->ESkillY = 0;
    this->ESkillUseTime = 0;
    this->isESkillParticleInAir = false;
    this->RBurstCloneSlashCount = 0;
    this->airDoubleJumped = false;
    this->airDashed = false;
    this->airASkillCloned = false;
    this->airESkillFloatCpt = 0;
    this->airPlungeLoopSoundChannel = 0;
    this->isLocked = false;

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
               128, 96, 15, 52);
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

    initSprite(KQ_RUN_NATK, "res/gfx/keqing/RunNAtk.png",
               160, 128, 11, 60);
    setXYShift(-40, -16, -60, KQ_RUN_NATK);
    setSpriteFrameLengthFromTo(30, 0, 3, KQ_RUN_NATK);
    setSpriteFrameLengthFromTo(40, 7, 10, KQ_RUN_NATK);

    initSprite(KQ_SKILL_FLIP, "res/gfx/keqing/SkillFlip.png",
               160, 128, 14, 48);
    setXYShift(-48, -16, -50, KQ_SKILL_FLIP);
    setSpriteFrameLengthFromTo(36, 0, 6, KQ_SKILL_FLIP);

    initSprite(KQ_SKILL_FLIP_LAND, "res/gfx/keqing/SkillFlipLand.png",
               96, 96, 3, 46);
    setXYShift(0, 0, -32, KQ_SKILL_FLIP_LAND);

    initSprite(KQ_SKILL_CLONE, "res/gfx/keqing/SkillClone.png",
               128, 96, 10, 60);
    setXYShift(-42, 0, -24, KQ_SKILL_CLONE);

    initSprite(KQ_SKILL, "res/gfx/keqing/Skill.png",
               128, 96, 13, 60);
    setXYShift(-32, 0, -36, KQ_SKILL);
    setSpriteFrameLengthFromTo(40, 0, 5, KQ_SKILL);

    initSprite(KQ_SKILL_AIMING, "res/gfx/keqing/SkillAiming.png",
               128, 96, 8, INT32_MAX);
    setXYShift(-32, 0, -36, KQ_SKILL_AIMING);

    initSprite(KQ_SKILL_SLASH, "res/gfx/keqing/SkillSlash.png",
               192, 128, 14, 60);
    setXYShift(-48, -16, -84, KQ_SKILL_SLASH);
    setSpriteFrameLengthFromTo(40, 0, 5, KQ_SKILL_SLASH);

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

    initSprite(KQ_AIR_DASH, "res/gfx/keqing/AirDash.png",
               96, 96, 6, 40);
    setXYShift(-10, 2, -26, KQ_AIR_DASH);

    initSprite(KQ_AIR_SKILL_CLONE, "res/gfx/keqing/AirSkillClone.png",
               128, 96, 10, 60);
    setXYShift(-42, 0, -24, KQ_AIR_SKILL_CLONE);

    initSprite(KQ_AIR_SKILL, "res/gfx/keqing/AirSkill.png",
               128, 128, 13, 60);
    setXYShift(-32, -32, -36, KQ_AIR_SKILL);
    setSpriteFrameLengthFromTo(40, 0, 5, KQ_AIR_SKILL);

    initSprite(KQ_AIR_SKILL_AIMING, "res/gfx/keqing/AirSkillAiming.png",
               128, 128, 8, INT32_MAX);
    setXYShift(-32, -32, -36, KQ_AIR_SKILL_AIMING);

    initSprite(KQ_AIR_SKILL_SLASH, "res/gfx/keqing/AirSkillSlash.png",
               192, 128, 14, 60);
    setXYShift(-40, -16, -84, KQ_AIR_SKILL_SLASH);
    setSpriteFrameLengthFromTo(40, 0, 5, KQ_AIR_SKILL_SLASH);

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

void Keqing::setSoundSheetStartPath() {
    std::string soundStartPath = "res/sfx/keqing/" + Global::userData[DATA_KQ_VOICE_LANG];
    soundSheet->setSoundStartPath(soundStartPath);
}

void Keqing::setSoundSheet() {
    setSoundSheetStartPath();

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
    soundSheet->copySoundPaths(KQ_AIR_SKILL, KQ_SKILL);

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

    jumpPressTime = 0;
    ASkillFlipPressTime = 0;
    ASkillCloneCenterX = 0;
    ASkillCloneCenterY = 0;
    ESkillPausedSpriteCode = 0;
    ESkillCursorSoundChannel = 0;
    ESkillX = 0;
    ESkillY = 0;
    ESkillUseTime = 0;
    isESkillParticleInAir = false;
    RBurstCloneSlashCount = 0;
    airDoubleJumped = false;
    airDashed = false;
    airASkillCloned = false;
    airESkillFloatCpt = 0;
    airPlungeLoopSoundChannel = 0;
    isLocked = false;
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
        !isSpriteAnimated(KQ_DASH) &&
        !isSpriteAnimated(KQ_SKILL_FLIP_LAND) &&
        !isSpriteAnimated(KQ_SKILL_CLONE)) {
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
    if (!isInAir()) {
        if (isSpriteAnimated(KQ_JUMP)) {
            setSpriteAnimated(false, KQ_JUMP);

            setSpriteAnimated(true, KQ_JUMP_END);

            Sound::playAudioChunk("res/sfx/particle/KQJumpEnd.ogg");
        }
        airDoubleJumped = false;
        airDashed = false;
        airASkillCloned = false;
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
            cancelGravity();
            yVelocity += gravityWeight * (double) Global::dt;
        }
    }

    if (isSpriteAnimated(KQ_JUMP_END)) {
        xVelocity = 0;
        yVelocity = 0;
    }
}

const double NAtkVelocity[NAtkMax + 1] = {0.1, 0, 0.2,
                                          0, 0.4};
const char *NAtkSfxPaths[NAtkMax + 1] = {"res/sfx/particle/KQNAtk0.ogg",
                                         "res/sfx/particle/KQNAtk1.ogg",
                                         "res/sfx/particle/KQNAtk2.ogg",
                                         "res/sfx/particle/KQNAtk3.ogg",
                                         "res/sfx/particle/KQNAtk4.ogg"};

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
            if (isMouseLeftLong()) {
                setSpriteAnimated(false, KQ_NATK);
                setSpriteAnimated(true, KQ_CATK);
                return;
            }

            xVelocity = 0;
            if (isMouseLeftRecent()) { // Continue NAtk
                goToFrame(NAtkEndFrame[i] + 2, KQ_NATK);
                xVelocity = NAtkVelocity[i + 1];

                Sound::playAudioChunk(NAtkSfxPaths[i + 1]);
            }
        }
    }

    if (isNewestFrame(0, KQ_NATK)) {
        xVelocity = NAtkVelocity[0];

        soundSheet->playRandomSoundStartString("AtkWeak", KQ_NATK);
        Sound::playAudioChunk(NAtkSfxPaths[0]);

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

void Keqing::checkESkillOnCAtk() {
    if (Particle::isActive(PARTICLE_KQ_SKILL_IDLE)) {
        Particle *skillIdleParticle =
                Particle::getParticle(PARTICLE_KQ_SKILL_IDLE);
        Particle *skillExplosionParticle =
                Particle::pushParticle(PARTICLE_KQ_SKILL_EXPLOSION, 30);
        skillExplosionParticle->moveToEntityCenter(skillIdleParticle);

        Particle::removeParticle(PARTICLE_HUD_SKILL_ICON_2);
    }
}

void Keqing::CAtk() {
    if (isNewestFrame(6, KQ_CATK)) {
        xVelocity = -0.4;

        Particle *CAtkParticle =
                Particle::pushParticle(PARTICLE_KQ_CATK, 60);
        CAtkParticle->moveToEntityCenter(this);
        CAtkParticle->xyShift(30, 10);

        soundSheet->playRandomSound(KQ_CATK);
        Sound::playAudioChunk("res/sfx/particle/KQCAtK.ogg");

        checkESkillOnCAtk();
    }

    if (xVelocity < 0) {
        xVelocity = std::min(xVelocity + 0.001 * Global::dt, 0.0);
    }

    if (Particle::isActive(PARTICLE_KQ_CATK)) {
        // Push Atk
        Particle *CAtkParticle =
                Particle::getParticle(PARTICLE_KQ_CATK);
        if (CAtkParticle->isNewestFrame(1)) {
            const int N = 20;
            double atkPolyPts[N][2];
            approxEllipse(atkPolyPts, N,
                          0, 0, 60, 30);
            shiftXYArray(atkPolyPts, N,
                         10, 0);
            Attack *atk =
                    Global::currentWorld->addKQAtk(this, CAtkParticle,
                                                   atkPolyPts, N,
                                                   10, 2.6, -0.1);
            atk->setAtkDuration(CAtkParticle->getSpriteLengthFromTo(1, 1));

        } else if (CAtkParticle->isNewestFrame(3)) {
            const int N = 20;
            double atkPolyPts[N][2];
            approxEllipse(atkPolyPts, N,
                          0, 0, 60, 34);
            shiftXYArray(atkPolyPts, N,
                         20, 0);
            Attack *atk =
                    Global::currentWorld->addKQAtk(this, CAtkParticle,
                                                   atkPolyPts, N,
                                                   10, 2.6, -0.1);
            atk->setAtkDuration(CAtkParticle->getSpriteLengthFromTo(3, 3));
        }
    }
}

void Keqing::upNAtk() {
    if (isNewestFrame(0, KQ_UP_NATK)) {
        soundSheet->playRandomSound(KQ_UP_NATK);
        Sound::playAudioChunk("res/sfx/particle/KQNAtk1.ogg");
    }

    if (isFrameBetween(6, -1, KQ_UP_NATK)) {
        if (isKeyPressed(KEY_Z) && isKeyPressedLong(KEY_MOUSE_LEFT)) {
            setSpriteAnimated(false, KQ_UP_NATK);
            setSpriteAnimated(true, KQ_UP_CATK);
        }
    }

    // Push Atk
    if (isNewestFrame(2, KQ_UP_NATK)) {
        const int N = 14;
        double atkPolyPts[N][2] =
                {{-42, 44},
                 {-52, 0},
                 {-42, -46},
                 {-26, -70},
                 {-4,  -86},
                 {24,  -84},
                 {44,  -68},
                 {60,  -42},
                 {66,  10},
                 {56,  40},
                 {34,  -40},
                 {16,  -46},
                 {-8,  -36},
                 {-14, 22}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(2, 3, KQ_UP_NATK));
    }
}

void Keqing::upCAtk() {
    if (isNewestFrame(7, KQ_UP_CATK)) {
        Particle *upCAtkPartcle =
                Particle::pushParticle(PARTICLE_KQ_UP_CATK, 40);
        upCAtkPartcle->moveToEntityCenter(this);
        upCAtkPartcle->xyShift(0, -40);

        soundSheet->playRandomSound(KQ_UP_CATK);
        Sound::playAudioChunk("res/sfx/particle/KQCAtK.ogg");

        checkESkillOnCAtk();
    }

    if (Particle::isActive(PARTICLE_KQ_UP_CATK)) {
        // Push Atk
        Particle *upCAtkParticle =
                Particle::getParticle(PARTICLE_KQ_UP_CATK);
        if (upCAtkParticle->isNewestFrame(1)) {
            const int N = 20;
            double atkPolyPts[N][2];
            approxEllipse(atkPolyPts, N,
                          0, -16, 40, 56);
            Attack *atk =
                    Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                                   10, 0.1, -0.1);
            atk->setAtkDuration(upCAtkParticle->getSpriteLengthFromTo(1, 1));

        } else if (upCAtkParticle->isNewestFrame(3)) {
            const int N = 20;
            double atkPolyPts[N][2];
            approxEllipse(atkPolyPts, N,
                          0, -36, 26, 70);
            Attack *atk =
                    Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                                   10, 0.1, -0.8);
            atk->setAtkDuration(upCAtkParticle->getSpriteLengthFromTo(3, 3));
        }
    }
}

void Keqing::crouchNAtk() {
    if (isNewestFrame(0, KQ_CROUCH_NATK)) {
        soundSheet->playRandomSound(KQ_CROUCH_NATK);
        Sound::playAudioChunk("res/sfx/particle/KQNAtk0.ogg");
    }

    if (isFrameBetween(4, -1, KQ_CROUCH_NATK)) {
        if (isKeyPressed(KEY_S) && isKeyPressedLong(KEY_MOUSE_LEFT)) {
            setSpriteAnimated(false, KQ_CROUCH_NATK);
            setSpriteAnimated(true, KQ_CROUCH_CATK);
        }
    }

    // Push Atk
    if (isNewestFrame(1, KQ_CROUCH_NATK)) {
        const int N = 4;
        double atkPolyPts[N][2] =
                {{38,  14},
                 {136, 42},
                 {48,  42},
                 {48,  30}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(1, 2, KQ_CROUCH_NATK));
    }
}

void Keqing::crouchCAtk() {
    if (isNewestFrame(7, KQ_CROUCH_CATK)) {
        Particle *crouchCAtkParticle =
                Particle::pushParticle(PARTICLE_KQ_CROUCH_CATK, 40);
        crouchCAtkParticle->moveToEntityCenter(this);
        crouchCAtkParticle->xyShift(0, 24);

        soundSheet->playRandomSound(KQ_CROUCH_CATK);
        Sound::playAudioChunk("res/sfx/particle/KQCAtK.ogg");

        checkESkillOnCAtk();
    }

    if (Particle::isActive(PARTICLE_KQ_CROUCH_CATK)) {
        // Push Atk
        Particle *crouchCAtkParticle =
                Particle::getParticle(PARTICLE_KQ_CROUCH_CATK);
        if (crouchCAtkParticle->isNewestFrame(1)) {
            const int N = 20;
            double atkPolyPts[N][2];
            approxEllipse(atkPolyPts, N,
                          6, 10, 64, 18);
            Attack *atk =
                    Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                                   10, 0.4, -0.4);
            atk->setAtkDuration(crouchCAtkParticle->getSpriteLengthFromTo(1, 1));

        } else if (crouchCAtkParticle->isNewestFrame(3)) {
            const int N = 20;
            double atkPolyPts[N][2];
            approxEllipse(atkPolyPts, N,
                          0, 26, 120, 24);
            Attack *atk =
                    Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                                   10, 0.4, -0.4);
            atk->setAtkDuration(crouchCAtkParticle->getSpriteLengthFromTo(3, 3));
        }
    }
}

void Keqing::dash() {
    if (isNewestFrame(0, KQ_DASH)) {
        if (isKeyPressed(KEY_Q)) setFacingEast(true);
        else if (isKeyPressed(KEY_D)) setFacingEast(false);

        Particle *dashVanishParticle =
                Particle::pushParticle(PARTICLE_KQ_DASH_VANISH, 40);
        dashVanishParticle->moveToEntityCenter(this);

        soundSheet->playRandomSound(KQ_DASH);

    } else if (isNewestFrame(2, KQ_DASH)) {
        xVelocity = -KQ_DASH_VELOCITY;

    } else if (isFrameBetween(6, -1, KQ_DASH)) {
        if (xVelocity < 0) {
            xVelocity = std::min(xVelocity + 0.01 * (double) Global::dt, 0.0);
        }
    }
}

void Keqing::runNAtk() {
    if (isNewestFrame(0, KQ_RUN_NATK)) {
        xVelocity = 0.4;

    } else if (isNewestFrame(5, KQ_RUN_NATK)) {
        xVelocity = 0.52;
        yVelocity = -0.76;

        Sound::playAudioChunk("res/sfx/particle/KQNAtk2.ogg");
    }

    // Push Atk
    if (isNewestFrame(4, KQ_RUN_NATK)) {
        const int N = 10;
        double atkPolyPts[N][2] =
                {{46,  8},
                 {60,  42},
                 {22,  44},
                 {-4,  44},
                 {-32, 38},
                 {-52, 24},
                 {-52, 14},
                 {-26, -2},
                 {-34, 18},
                 {2,   24}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(4, 4, KQ_RUN_NATK));

    } else if (isNewestFrame(5, KQ_RUN_NATK)) {
        const int N = 14;
        double atkPolyPts[N][2] =
                {{-72, 22},
                 {-62, -8},
                 {-32, -42},
                 {4,   -64},
                 {42,  -64},
                 {74,  -46},
                 {88,  -22},
                 {82,  16},
                 {50,  42},
                 {28,  46},
                 {56,  -6},
                 {24,  -16},
                 {-2,  -6},
                 {-26, 14}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(5, 6, KQ_RUN_NATK));
    }
}

Particle *Keqing::pushParticleOnSkillBlink(Entity *centerEntity) {
    int blinkCodeArray[2] = {PARTICLE_KQ_SKILL_BLINK_0,
                             PARTICLE_KQ_SKILL_BLINK_1};
    int rd = Random::getRandomInt(0, 1);
    Particle *blinkParticle =
            Particle::pushParticle(blinkCodeArray[rd], 40);
    blinkParticle->setRotation(Random::getRandomReal(0, 360));
    blinkParticle->moveToEntityCenter(centerEntity);

    return blinkParticle;
}

void Keqing::pushElectroAura(Entity *srcEntity, Particle *srcParticle,
                             double renderWM, double renderHM) {
    // One of the Parameters should be nullptr
    if (srcEntity != nullptr && srcParticle != nullptr) return;

    if (srcEntity != nullptr) {
        Particle *auraParticle =
                Particle::pushParticle(PARTICLE_KQ_ELECTRO_AURA,
                                       100,
                                       renderWM, renderHM);
        auraParticle->setRGBAMod(100);
        auraParticle->moveToEntityCenter(srcEntity);
        auraParticle->xyShift(-1, 0);
    }

    if (srcParticle != nullptr) {
        for (int i = 0; i < 6; i++) {
            Particle *auraParticle =
                    Particle::pushParticle(PARTICLE_KQ_ELECTRO_AURA,
                                           100,
                                           renderWM, renderHM);
            auraParticle->setRGBAMod(60);
            auraParticle->setRotation(i * 60);
            auraParticle->xyShift(-1, 10);
            auraParticle->goToFrame(i);

            auraParticle->moveToEntityCenter(srcParticle);
            auto *fParams = new int;
            *fParams = srcParticle->getCode();

            auraParticle->setOnRender([](Particle *particle) {
                auto *srcParticleCode = (int *) particle->getOnRenderParams();
                if (Particle::isActive(*srcParticleCode)) {
                    if (particle->isNewestFrame(6)) {
                        particle->goToFrameNoNew(2);
                    }

                } else {
                    particle->removeSelf();
                }
            }, (void *) fParams);

            auraParticle->setOnRemove([](Particle *particle) {
                auto *fParams = (int *) particle->getOnRenderParams();
                delete fParams;
            });

        }
    }
}

void Keqing::ASkillFlip() {
    if (isSpriteAnimated(KQ_SKILL_FLIP)) {
        if (isNewestFrame(0, KQ_SKILL_FLIP)) {
            ASkillFlipPressTime = Global::pressedTime[KEY_A];

            yVelocity = -KQ_SKILL_FLIP_BASE_Y_VELOCITY;
            xVelocity = KQ_SKILL_FLIP_BASE_X_VELOCITY;

        }

        if (yVelocity > -0.12 && yVelocity < 0.12) {
            cancelGravity(1.0 / 2.0);
        }

        if (isFrameBetween(3, 6, KQ_SKILL_FLIP)) {
            if (yVelocity > 0.32) {
                goToFrame(7, KQ_SKILL_FLIP);
            }

            if (isKeyPressed(KEY_A) && ASkillFlipPressTime != Global::pressedTime[KEY_A]) {
                if (ASkillFlipPressTime != Global::pressedTime[KEY_A]) {
                    goToFrame(7, KQ_SKILL_FLIP);
                }
            }
        }

        if (willFrameFinish(6, KQ_SKILL_FLIP)) {
            goToFrameNoNew(3, KQ_SKILL_FLIP);
        }

        if (isNewestFrame(7, KQ_SKILL_FLIP)) {
            Particle *flipSlashParticle =
                    Particle::pushParticle(PARTICLE_KQ_SKILL_FLIP_SLASH, 40);
            flipSlashParticle->moveToEntityCenter(this);
            flipSlashParticle->xyShift(50, 44);

            // Push Atk

            // Particle Slash Atk
            const int PtN = 20;
            double PtAtkPolyPts[PtN][2];
            approxEllipse(PtAtkPolyPts, PtN,
                          0, 0, 12, 84);
            rotateXYArray(PtAtkPolyPts, PtN,
                          0, 0,
                          (0.96 * M_PI) / 4.0, false);
            Attack *PtAtk =
                    Global::currentWorld->addKQAtk(this, flipSlashParticle,
                                                   PtAtkPolyPts, PtN,
                                                   10, 0.4, -0.4);
            PtAtk->setAtkDuration(flipSlashParticle->getSpriteLengthFromTo(0, 1));

            // KQ Slash Atk
            const int KqN = 20;
            double KqAtkPolyPts[KqN][2] =
                    {{42,  40},
                     {64,  26},
                     {78,  8},
                     {78,  -14},
                     {64,  -28},
                     {52,  -32},
                     {30,  -36},
                     {0,   -36},
                     {-26, -28},
                     {-52, -18},
                     {-74, -6},
                     {-42, 10},
                     {-50, -6},
                     {-36, -14},
                     {4,   -26},
                     {40,  -28},
                     {54,  -24},
                     {66,  -16},
                     {72,  -4},
                     {66,  18}};
            Attack *KqAtk =
                    Global::currentWorld->addKQAtk(this, KqAtkPolyPts, KqN,
                                                   10, 0.4, -0.4);
            KqAtk->setAtkDuration(getSpriteLengthFromTo(7, 8, KQ_SKILL_FLIP));
        }

        if (!isInAir() && !isNewestFrame(0, KQ_SKILL_FLIP)) {
            setSpriteAnimated(false, KQ_SKILL_FLIP);
            setSpriteAnimated(true, KQ_SKILL_FLIP_LAND);

            xVelocity = KQ_SKILL_FLIP_BASE_X_VELOCITY / 2.2;
        }
    }
}

void Keqing::ASkillCloneGeneral() {
    int SKILL_CLONE_CODE = (!isInAir()) ? KQ_SKILL_CLONE : KQ_AIR_SKILL_CLONE;

    if (isNewestFrame(0, SKILL_CLONE_CODE)) {
        getSelfCenter(&ASkillCloneCenterX, &ASkillCloneCenterY);

    } else if (isNewestFrame(3, SKILL_CLONE_CODE)) {
        xVelocity = -KQ_SKILL_CLONE_X_VELOCITY;

    } else if (isNewestFrame(4, SKILL_CLONE_CODE)) {
        if (!Particle::isActive(PARTICLE_KQ_SKILL_CLONE_SPAWN) &&
            !Particle::isActive(PARTICLE_KQ_SKILL_CLONE_IDLE) &&
            !Particle::isActive(PARTICLE_KQ_SKILL_CLONE_DESPAWN)) {

            Particle *cloneSpawnParticle =
                    Particle::pushParticle(PARTICLE_KQ_SKILL_CLONE_SPAWN, 60);
            cloneSpawnParticle->setFacingEast(isFacingEast());
            cloneSpawnParticle->moveToCenter(ASkillCloneCenterX, ASkillCloneCenterY);
            cloneSpawnParticle->xyShift(16, 2);

            cloneSpawnParticle->setOnRender([](Particle *particle) {
                if (particle->willFrameFinish(3)) {
                    Keqing *kq = Keqing::getInstance();

                    Particle *cloneIdleParticle =
                            Particle::pushParticle(PARTICLE_KQ_SKILL_CLONE_IDLE, 40);
                    cloneIdleParticle->setSpriteFrameLengthFromTo(880, 0, 0);
                    cloneIdleParticle->moveToEntityCenter(particle);
                    SDL_Rect kqHitBox = kq->getHitBox();
                    double cIPtRealW;
                    cloneIdleParticle->getRealSize(&cIPtRealW, nullptr);
                    int hitBoxX = kqHitBox.x + (int) (cIPtRealW - kqHitBox.w) * !cloneIdleParticle->isFacingEast();
                    cloneIdleParticle->setHitBox({hitBoxX, 4,
                                                  kqHitBox.w, kqHitBox.h});

                    cloneIdleParticle->setOnRender([](Particle *particle) {
                        if (particle->getTimeSinceCreation() > KQ_SKILL_CLONE_DURATION) {
                            particle->removeSelf();
                            return;
                        }

                        if (particle->isNewestFrame(1)) {
                            Keqing::pushParticleOnSkillBlink(particle);
                        }
                    });

                    cloneIdleParticle->setOnRemove([](Particle *particle) {
                        Particle *cloneDespawnParticle =
                                Particle::pushParticle(PARTICLE_KQ_SKILL_CLONE_DESPAWN, 60);
                        cloneDespawnParticle->moveToEntityCenter(particle);
                    });

                    Keqing::pushElectroAura(nullptr, cloneIdleParticle,
                                            0.92, 1.48);

                    // Push Atk
                    const int N = 8;
                    double atkPolyPts[N][2] =
                            {{-22, -36},
                             {22,  -36},
                             {22,  10},
                             {64,  38},
                             {60,  38},
                             {22,  26},
                             {22,  40},
                             {-22, 40}};
                    Attack *atk =
                            Global::currentWorld->addKQAtk(kq, cloneIdleParticle,
                                                           atkPolyPts, N,
                                                           10, 0.1, -0.1);

                    atk->setOnHit([](Attack *atk, void *fParams) {
                        auto *cloneIdleParticle = (Particle *) fParams;
                        cloneIdleParticle->removeSelf();
                    }, (void *) cloneIdleParticle);

                    atk->setShouldRemove([](Attack *atk, void *fParams) {
                        return (!Particle::isActive(PARTICLE_KQ_SKILL_CLONE_IDLE));
                    }, nullptr);

                }
            });
        }
    }

    for (int i = 3; i <= 6; i++) {
        if (isNewestFrame(i, SKILL_CLONE_CODE)) {
            Particle *kqFadeParticle =
                    Particle::pushParticle(PARTICLE_KQ_SKILL_CLONE_FADE, 30);
            kqFadeParticle->moveToEntityCenter(this);
            kqFadeParticle->xyShift(-4, -6);
            if (isInAir()) kqFadeParticle->xyShift(0, -8);

            if ((i % 2) == 0) {
                Particle *blinkParticle = pushParticleOnSkillBlink(this);
                blinkParticle->xyShift(-4, -6);
                if (isInAir()) blinkParticle->xyShift(0, -8);
            }
        }
    }

    xVelocity = std::min(xVelocity + 0.001 * Global::dt, 0.0);
}

void Keqing::ASkillClone() {
    ASkillCloneGeneral();
}

void Keqing::updateSkillHudIcon() {
    Particle *skillCircleHud =
            Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE);

    Particle *skillIcon2 =
            Particle::pushParticle(PARTICLE_HUD_SKILL_ICON_2,
                                   KQ_LIGHTNING_STILETTO_DURATION,
                                   HUB_SB_ICON_M, HUB_SB_ICON_M);
    skillIcon2->moveToEntityCenter(skillCircleHud);

    skillIcon2->setOnRemove([](Particle *removedParticle) {
        Particle *skillCircleHud =
                Particle::getParticle(PARTICLE_HUD_SKILL_CIRCLE);

        Particle *skillIcon1 =
                Particle::pushParticle(PARTICLE_HUD_SKILL_ICON_1,
                                       INT32_MAX,
                                       HUB_SB_ICON_M, HUB_SB_ICON_M);
        skillIcon1->moveToEntityCenter(skillCircleHud);
        skillIcon1->setRGBAMod(COLOR_WHITE, HUD_SB_USED_ALPHA);

        int elapsedTime = getTime() - Keqing::getInstance()->getESkillUseTime();
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
            skillCircleHud->setRGBAMod(HUD_SKILL1_CIRCLE_RGBA);
            Particle *skillIcon1Hud = Particle::getParticle(PARTICLE_HUD_SKILL_ICON_1);
            skillIcon1Hud->setRGBAMod(COLOR_WHITE_FULL);
        });

        Particle::removeParticle(PARTICLE_HUD_SKILL_CIRCLE);

        Particle::removeParticle(PARTICLE_KQ_SKILL_IDLE);
    });

    Particle::removeParticle(PARTICLE_HUD_SKILL_ICON_1);
    skillCircleHud->setRGBAMod(HUD_SKILL2_CIRCLE_RGBA);
}

void Keqing::createSkillSpawnParticle() {
    Keqing *kq = Keqing::getInstance();

    Particle *skillShootParticle =
            Particle::pushParticle(PARTICLE_KQ_SKILL_SHOOT, 30);
    skillShootParticle->moveToEntityCenter(kq);
    skillShootParticle->moveAdd((kq->isFacingEast()) ? 30 : -30, 10);

    Particle *spawnParticle =
            Particle::pushParticle(PARTICLE_KQ_SKILL_SPAWN, 60);
    spawnParticle->moveToCenter(kq->getESkillX(), kq->getESkillY());
    spawnParticle->setHitBoxCentered((int) kq->getBaseHitBoxW(),
                                     (int) kq->getBaseHitBoxH());
    spawnParticle->findNearestSurface();
    spawnParticle->pauseSprite(true);
}

void Keqing::createLightningStelitto() {
    Particle *spawnParticle =
            Particle::getParticle(PARTICLE_KQ_SKILL_SPAWN);

    Particle *idleParticle =
            Particle::pushParticle(PARTICLE_KQ_SKILL_IDLE,
                                   40, 2.0, 2.0);
    idleParticle->setSpriteFrameLengthFromTo(880, 0, 0);
    idleParticle->moveToEntityCenter(spawnParticle);
    idleParticle->setHitBoxCentered(spawnParticle->getHitBox().w,
                                    spawnParticle->getHitBox().h);
    idleParticle->goToFrameNoNew(1);

    idleParticle->setOnRender([](Particle *particle) {
        SDL_Rect particleRect = particle->getHitBox();
        particleRect.x += (int) particle->getX();
        particleRect.y += (int) particle->getY();
        SDL_Renderer *gRenderer = WindowRenderer::getInstance()->getRenderer();
        const int rectH = 2;
        SDL_Rect dstRect = {particleRect.x,
                            particleRect.y + particleRect.h,
                            particleRect.w,
                            rectH};
        WindowRenderer::renderRect(&dstRect, true,
                                   COLOR_BLACK_FULL,
                                   gRenderer,
                                   false, true);

        if (particle->isNewestFrame(1)) {
            Keqing::pushParticleOnSkillBlink(particle);
        }
    });

    idleParticle->setOnRemove([](Particle *particle) {
        Particle *skillDespawnParticle =
                Particle::pushParticle(PARTICLE_KQ_SKILL_DESPAWN, 60);
        skillDespawnParticle->moveToEntityCenter(particle);
    });

    Keqing::pushElectroAura(nullptr, idleParticle);
}

void Keqing::createSkillProjParticle() {
    Keqing *kq = Keqing::getInstance();
    Particle *spawnParticle =
            Particle::getParticle(PARTICLE_KQ_SKILL_SPAWN);

    Particle *projParticle =
            Particle::pushParticle(PARTICLE_KQ_SKILL_PROJ, INT32_MAX);

    double kqCenterX, kqCenterY;
    kq->getSelfCenter(&kqCenterX, &kqCenterY);

    auto *spawnParticleCenterX = new double;
    auto *spawnParticleCenterY = new double;
    spawnParticle->getSelfCenter(spawnParticleCenterX, spawnParticleCenterY);

    double angle = getAngle(kqCenterX, kqCenterY,
                            *spawnParticleCenterX, *spawnParticleCenterY,
                            true, !kq->isFacingEast());

    projParticle->setRotation(radToDegree(angle));
    projParticle->moveToEntityCenter(kq);
    projParticle->setHitBoxCentered(spawnParticle->getHitBox().w,
                                    spawnParticle->getHitBox().h);
//    projParticle->setXYVelocity(KQ_LIGHTNING_STELITTO_VELOCITY * cos(angle),
//                                KQ_LIGHTNING_STELITTO_VELOCITY * sin(angle));

    auto *fParams = new f2Params;
    fParams->fParam0 = (void *) spawnParticleCenterX;
    fParams->fParam1 = (void *) spawnParticleCenterY;
    projParticle->setOnRender([](Particle *particle) {
        double lastCenterX, lastCenterY;
        particle->getSelfCenter(&lastCenterX, &lastCenterY);

        double angleDeg = particle->getRotation();
        double angle = degreeToRad(angleDeg);

        Particle *projAftFxParticle =
                Particle::pushParticle(PARTICLE_KQ_SKILL_PROJ_AFT_FX, 20);
        projAftFxParticle->setRotation(angleDeg);
        projAftFxParticle->moveToEntityCenter(particle);

        double distance = KQ_LIGHTNING_STELITTO_VELOCITY * (double) Global::dt;
        double addX = cos(angle) * distance;
        if (!particle->isFacingEast()) addX = -addX;
        double addY = sin(angle) * distance;
        particle->moveAdd(addX, addY);
//        particle->moveX();
//        particle->moveY();

        auto *fParams = (f2Params *) particle->getOnRenderParams();
        double dstX = *((double *) fParams->fParam0);
        double dstY = *((double *) fParams->fParam1);
        double newCenterX, newCenterY;
        particle->getSelfCenter(&newCenterX, &newCenterY);

//        if ((lastCenterX == newCenterX && particle->getXVelocity() != 0) ||
//            (lastCenterY == newCenterY && particle->getYVelocity() != 0) ||
        if ((lastCenterX < dstX && newCenterX >= dstX) ||
            (lastCenterX > dstX && newCenterX <= dstX) ||
            (lastCenterY < dstY && newCenterY >= dstY) ||
            (lastCenterY > dstY && newCenterY <= dstY)) {
            Particle::removeParticle(PARTICLE_KQ_SKILL_PROJ);

            Particle *spawnParticle =
                    Particle::getParticle(PARTICLE_KQ_SKILL_SPAWN);
//            spawnParticle->moveToEntityCenter(particle);
            spawnParticle->pauseSprite(false);

            Keqing::createLightningStelitto();

            Sound::playAudioChunk("res/sfx/particle/KQSkillSpawn.ogg");
        }
    }, fParams);

    projParticle->setOnRemove([](Particle *particle) {
        auto *fParams = (f2Params *) particle->getOnRenderParams();
        delete ((double *) fParams->fParam0);
        delete ((double *) fParams->fParam1);
        delete fParams;
    });

    // Push Atk
    const int N = 4;
    double atkPolyPts[N][2] =
            {{-10, -10},
             {24,  0},
             {-10, 12},
             {-26, 0}};
    double kbXV = cos(angle);
    double kbYV = sin(angle);
    Attack *atk =
            Global::currentWorld->addKQAtk(kq, projParticle,
                                           atkPolyPts, N,
                                           10, kbXV, kbYV);
    atk->setShouldRemove([](Attack *atk, void *fParams) {
        return (!Particle::isActive(PARTICLE_KQ_SKILL_PROJ));
    }, nullptr);
}

void Keqing::getESkillSpriteCodes(int *pSkill, int *pSkillAiming, int *pSkillSlash) {
    if (pSkill != nullptr) *pSkill = (!isInAir()) ? KQ_SKILL : KQ_AIR_SKILL;
    if (pSkillAiming != nullptr) *pSkillAiming = (!isInAir()) ? KQ_SKILL_AIMING : KQ_AIR_SKILL_AIMING;
    if (pSkillSlash != nullptr) *pSkillSlash = (!isInAir()) ? KQ_SKILL_SLASH : KQ_AIR_SKILL_SLASH;
}

void Keqing::ESkillGeneral() {
    int SKILL_CODE, SKILL_AIMING_CODE;
    getESkillSpriteCodes(&SKILL_CODE, &SKILL_AIMING_CODE, nullptr);

    if (isNewestFrame(0, SKILL_CODE)) {
        Sound::playAudioChunk("res/sfx/particle/KQSkillStart.ogg");

    } else if (isNewestFrame(6, SKILL_CODE)) {
        ESkillUseTime = Global::currentTime;

        Keqing::updateSkillHudIcon();

        Keqing::createSkillSpawnParticle();

        Keqing::createSkillProjParticle();

        Sound::deleteAudioChunk(ESkillCursorSoundChannel);
        soundSheet->playRandomSound(SKILL_CODE);
        Sound::playAudioChunk("res/sfx/particle/KQSkillShoot.ogg");
    }

    if (willFrameFinish(3, SKILL_CODE)) {
        if (!isKeyPressedLong(KEY_E)) {
            double kqCenterX, kqCenterY;
            getSelfCenter(&kqCenterX, &kqCenterY);
            ESkillX = (isFacingEast()) ? kqCenterX + KQ_SKILL_TP_MAX_DISTANCE : kqCenterX - KQ_SKILL_TP_MAX_DISTANCE;
            ESkillY = kqCenterY;
        } else {
            pauseSprite(true, SKILL_CODE);
            ESkillPausedSpriteCode = SKILL_CODE;
            setSpriteAnimated(true, SKILL_AIMING_CODE);
        }
    }
}

void Keqing::ESkill() {
    ESkillGeneral();
}

void Keqing::ESkillAimingGeneral() {
    int SKILL_CODE, SKILL_AIMING_CODE;
    getESkillSpriteCodes(&SKILL_CODE, &SKILL_AIMING_CODE, nullptr);

    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);
    getMouseAbsoluteXYNotTranslated(&mouseX, &mouseY);

    double kqCenterX, kqCenterY;
    getSelfCenter(&kqCenterX, &kqCenterY);
    double angle = getAngle(kqCenterX, kqCenterY,
                            mouseX, mouseY,
                            false, !isFacingEast());

    int frameIndex = (int) ((angle / (2.0 * M_PI)) * 8);
    goToFrame(frameIndex, SKILL_AIMING_CODE);

    double dist = getDistance(kqCenterX, kqCenterY,
                              (double) mouseX, (double) mouseY);
    if (dist > KQ_SKILL_TP_MAX_DISTANCE) {
        double absX = cos(angle) * KQ_SKILL_TP_MAX_DISTANCE;
        if (!isFacingEast()) absX = -absX;
        mouseX = (int) (kqCenterX + absX);
        mouseY = (int) (kqCenterY - sin(angle) * KQ_SKILL_TP_MAX_DISTANCE);
    }

    if (!Particle::isActive(PARTICLE_KQ_SKILL_CURSOR)) {
        Particle *cursorIdleParticle =
                Particle::getParticle(PARTICLE_KQ_SKILL_CURSOR);
        Particle *cursorSpawnParticle =
                Particle::pushParticle(PARTICLE_KQ_SKILL_CURSOR_SPAWN,
                                       30, 1.2, 1.2);
        cursorSpawnParticle->moveToCenter((double) mouseX, (double) mouseY);
        cursorSpawnParticle->xyShift(-2, 0);

        cursorIdleParticle =
                Particle::pushParticle(PARTICLE_KQ_SKILL_CURSOR,
                                       60, 2.0, 2.0);
        cursorIdleParticle->goToFrame(4);

        ESkillCursorSoundChannel =
                Sound::playAudioChunk("res/sfx/particle/KQSkillAiming.ogg", INT32_MAX);
    }

    Particle *cursorIdleParticle =
            Particle::getParticle(PARTICLE_KQ_SKILL_CURSOR);
    cursorIdleParticle->moveToCenter((double) mouseX, (double) mouseY);

    if (!isKeyPressed(KEY_E)) {
        Particle::removeParticle(PARTICLE_KQ_SKILL_CURSOR);

        ESkillX = (double) mouseX;
        ESkillY = (double) mouseY;

        setSpriteAnimated(false, SKILL_AIMING_CODE);
        pauseSprite(false, ESkillPausedSpriteCode);
        goToFrameNoNew(4, ESkillPausedSpriteCode);
    }
}

void Keqing::ESkillAiming() {
    ESkillAimingGeneral();
}

void Keqing::ESkillSlashGeneral() {
    int SKILL_SLASH_CODE;
    getESkillSpriteCodes(nullptr, nullptr, &SKILL_SLASH_CODE);

    if (isNewestFrame(0, SKILL_SLASH_CODE)) {
        Particle *idleParticle =
                Particle::getParticle(PARTICLE_KQ_SKILL_IDLE);
        if (x <= idleParticle->getX()) setFacingEast(true);
        else setFacingEast(false);

        getToEntityCenterXY(idleParticle, &ESkillX, &ESkillY);

        isESkillParticleInAir = idleParticle->isInAir();

        Particle::removeParticle(PARTICLE_HUD_SKILL_ICON_2);

    } else if (isNewestFrame(6, SKILL_SLASH_CODE)) {
        Particle *tpStartParticle =
                Particle::pushParticle(PARTICLE_KQ_SKILL_TP_START, 60);
        tpStartParticle->moveToEntityCenter(this);

        Keqing::pushElectroAura(this, nullptr,
                                1.42, 1.42);

        moveTo(ESkillX, ESkillY);

        Particle *tpEndParticle =
                Particle::pushParticle(PARTICLE_KQ_SKILL_TP_END, 60);
        tpEndParticle->moveToEntityCenter(this);

        soundSheet->playRandomSound(SKILL_SLASH_CODE);
        Sound::playAudioChunk("res/sfx/particle/KQSkillTP.ogg");

        if (isESkillParticleInAir && SKILL_SLASH_CODE == KQ_SKILL_SLASH) {
            setSpriteAnimated(false, SKILL_SLASH_CODE);
            setSpriteAnimated(true, KQ_AIR_SKILL_SLASH);
            goToFrameNoNew(6, KQ_AIR_SKILL_SLASH);
        }
        if (!isESkillParticleInAir && SKILL_SLASH_CODE == KQ_AIR_SKILL_SLASH) {
            setSpriteAnimated(false, SKILL_SLASH_CODE);
            setSpriteAnimated(true, KQ_SKILL_SLASH);
            goToFrameNoNew(6, KQ_SKILL_SLASH);
        }
    }

    // Push Atk
    if (isNewestFrame(7, SKILL_SLASH_CODE)) {
        const int N = 8;
        double atkPolyPts[N][2] =
                {{-50, -36},
                 {60,  -36},
                 {122, -10},
                 {122, 22},
                 {60,  46},
                 {-50, 46},
                 {-76, 22},
                 {-76, -10}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(7, 9, SKILL_SLASH_CODE));
    }
}

void Keqing::ESkillSlash() {
    ESkillSlashGeneral();
}

void pushBurstSlashAtk(int dmg, double kbXV, double kbYV,
                       int duration) {
    Keqing *kq = Keqing::getInstance();

    const int N = 40;
    double atkPolyPts[N][2];
    approxEllipse(atkPolyPts, N,
                  0, 0, 200, 200);
    Attack *atk =
            Global::currentWorld->addKQAtk(kq, atkPolyPts, N,
                                           dmg, kbXV, kbYV);
    atk->setAtkDuration(duration);
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

    int cSlashCount = kq->getRBurstCloneSlashCount();
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

        // Push Atk
        pushBurstSlashAtk(10, 0.1, -0.1,
                          cSlashParticle->getSpriteLengthFromTo(0, -1));
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

    } else if (isNewestFrame(7, KQ_BURST)) {
        // Push Atk
        pushBurstSlashAtk(0, 0, 0, 40);

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

        // Push Atk
        pushBurstSlashAtk(20, 1.0, -1.0,
                          finalSlashParticle->getSpriteLengthFromTo(0, 4));
    }

    // Enlarge Circle
    if (Particle::isActive(PARTICLE_KQ_BURST_AOE)) {
        Particle *aoeParticle =
                Particle::getParticle(PARTICLE_KQ_BURST_AOE);
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
                Particle *aoeParticle =
                        Particle::getParticle(PARTICLE_KQ_BURST_AOE);
                Particle *slashParticle =
                        Particle::pushParticle(PARTICLE_KQ_BURST_SLASH,
                                               slashDuration,
                                               1.2, 1.2);
                slashParticle->moveToEntityCenter(aoeParticle);
                slashParticle->setRotation(slashRotations[i]);
                slashParticle->delaySprite(2 * slashDuration * i);
                slashParticle->setOnRender([](Particle *particle) {
                    if (particle->isNewestFrame(0)) {
                        // Push Atk
                        pushBurstSlashAtk(10, 0.1, -0.1,
                                          particle->getSpriteLengthFromTo(0, -1));
                    }
                });
            }
        }
    }
}

void Keqing::airDoubleJump() {
    if (isNewestFrame(0, KQ_AIR_DOUBLE_JUMP)) {
        airDoubleJumped = true;
        yVelocity = -KQ_AIR_DOUBLE_JUMP_BASE_VELOCITY;

        soundSheet->playRandomSound(KQ_AIR_DOUBLE_JUMP);
    }
}

void Keqing::airNAtk() {
    if (!isInAir()) {
        setSpriteAnimated(false, KQ_AIR_NATK);
        return;
    }

    if (isNewestFrame(0, KQ_AIR_NATK)) {
        soundSheet->playRandomSound(KQ_AIR_NATK);

    } else if (isNewestFrame(2, KQ_AIR_NATK)) {
        Sound::playAudioChunk("res/sfx/particle/KQNAtk0.ogg");

    } else if (isNewestFrame(5, KQ_AIR_NATK)) {
        Sound::playAudioChunk("res/sfx/particle/KQNAtk1.ogg");
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
    if (!isInAir()) {
        setSpriteAnimated(false, KQ_AIR_UP_NATK);
        return;
    }

    if (isNewestFrame(0, KQ_AIR_UP_NATK)) {
        soundSheet->playRandomSound(KQ_AIR_UP_NATK);
        Sound::playAudioChunk("res/sfx/particle/KQNAtk2.ogg");
    }

    // Push Atk
    if (isNewestFrame(2, KQ_AIR_UP_NATK)) {
        const int N = 6;
        double atkPolyPts[N][2] =
                {{28, -40},
                 {38, -24},
                 {42, 2},
                 {30, 26},
                 {8,  34},
                 {14, -6}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(2, 2, KQ_AIR_UP_NATK));

    } else if (isNewestFrame(3, KQ_AIR_UP_NATK)) {
        const int N = 8;
        double atkPolyPts[N][2] =
                {{6,  -108},
                 {14, -100},
                 {26, -76},
                 {32, -62},
                 {38, -32},
                 {40, -4},
                 {38, 12},
                 {-2, -64}};
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setAtkDuration(getSpriteLengthFromTo(3, 4, KQ_AIR_UP_NATK));
    }
}

void Keqing::airDash() {
    yVelocity = 0;

    if (isNewestFrame(0, KQ_AIR_DASH)) {
        airDashed = true;
        if (isKeyPressed(KEY_Q)) setFacingEast(true);
        else if (isKeyPressed(KEY_D)) setFacingEast(false);

        xVelocity = (isKeyPressed(KEY_Q) || isKeyPressed(KEY_D)) ? -KQ_AIR_DASH_VELOCITY : 0;

        Particle *dashVanishParticle =
                Particle::pushParticle(PARTICLE_KQ_DASH_VANISH, 40);
        dashVanishParticle->moveToEntityCenter(this);

        soundSheet->playRandomSound(KQ_AIR_DASH);
    }
}

void Keqing::airASkillClone() {
    if (isNewestFrame(0, KQ_AIR_SKILL_CLONE)) {
        airASkillCloned = true;
    }

    if (isFrameBetween(0, 6, KQ_AIR_SKILL_CLONE)) {
        yVelocity = 0;
    }

    ASkillCloneGeneral();

    if (!isInAir()) {
        yVelocity = 0;
        relaySprite(KQ_SKILL_CLONE, KQ_AIR_SKILL_CLONE);
    }
}

void Keqing::airESkill() {
    if (isNewestFrame(0, KQ_AIR_SKILL)) {
        yVelocity = 0.2;

    } else if (isNewestFrame(6, KQ_AIR_SKILL)) {
        yVelocity = -0.56;
    }

    if (isFrameBetween(0, 5, KQ_AIR_SKILL)) {
        cancelGravity(1.0 / 2.0);
    }

    ESkillGeneral();

    if (!isInAir()) {
        yVelocity = 0;
        relaySprite(KQ_SKILL, KQ_AIR_SKILL);
    }
}

void Keqing::airESkillAiming() {
    cancelGravity();

    if (!Particle::isActive(PARTICLE_KQ_SKILL_CURSOR)) {
        yVelocity = KQ_SKILL_AIR_AIMING_FLOAT_VELOCITY;
        airESkillFloatCpt = 0;
    }

    if (airESkillFloatCpt > KQ_SKILL_AIR_AIMING_FLOAT_FRAME_N) {
        yVelocity = (yVelocity < 0) ? KQ_SKILL_AIR_AIMING_FLOAT_VELOCITY : -KQ_SKILL_AIR_AIMING_FLOAT_VELOCITY;
        airESkillFloatCpt = 0;
    }
    airESkillFloatCpt++;

    ESkillAimingGeneral();

    if (!isInAir()) {
        yVelocity = 0;
        relaySprite(KQ_SKILL_AIMING, KQ_AIR_SKILL_AIMING);
    }
}

void Keqing::airESkillSlash() {
    yVelocity = 0;

    ESkillSlashGeneral();
}

void Keqing::airPlunge() { // Plunge Attack in Genshin
    if (isNewestFrame(0, KQ_AIR_PLUNGE)) {
        setSpriteAnimated(false, KQ_JUMP);

        soundSheet->playRandomSound(KQ_AIR_PLUNGE);
        Sound::playAudioChunk("res/sfx/particle/KQPlungeStart.ogg");

    } else if (isNewestFrame(1, KQ_AIR_PLUNGE)) {
        yVelocity = -0.4;

    } else if (isNewestFrame(6, KQ_AIR_PLUNGE)) {
        Particle *airPlungeParticle =
                Particle::pushParticle(PARTICLE_KQ_AIR_PLUNGE,
                                       60, 3.2, 0.1);
        airPlungeParticle->setEntity(this);
        airPlungeParticle->xyShift(26, 52);
        airPlungeParticle->setOnRender([](Particle *particle) {
            particle->addRenderWHMultiplierR(0, 0.02 * Global::dt,
                                             0, 2.0);
            Keqing *kq = Keqing::getInstance();
            if (!kq->isSpriteAnimated(KQ_AIR_PLUNGE) || !kq->isInAir()) {
                Particle::removeParticle(PARTICLE_KQ_AIR_PLUNGE);
                Sound::deleteAudioChunk(kq->getAirPlungeLoopSoundChannel());
            }
        });

        yVelocity = 0.2;

        airPlungeLoopSoundChannel = Sound::playAudioChunk(
                "res/sfx/particle/KQPlungeLoop.ogg", INT32_MAX);
    }

    if (willFrameFinish(7, KQ_AIR_PLUNGE)) {
        if (isInAir()) {
            goToFrameNoNew(6, KQ_AIR_PLUNGE);
        }
    }

    if (isFrameBetween(6, 7, KQ_AIR_PLUNGE)) {
        if (!isInAir()) {
            goToFrame(8, KQ_AIR_PLUNGE);

            Particle *airPlungeGroundParticle =
                    Particle::pushParticle(PARTICLE_KQ_AIR_PLUNGE_GROUND, 60);
            airPlungeGroundParticle->moveToEntityCenter(this);
            airPlungeGroundParticle->xyShift(20, -6);

            Sound::deleteAudioChunk(airPlungeLoopSoundChannel);
            Sound::playAudioChunk("res/sfx/particle/KQPlungeEnd.ogg");

            // Push Atk
            const int N = 4;
            double atkPolyPts[N][2] =
                    {{-46, 0},
                     {86,  0},
                     {86,  42},
                     {-46, 42}};
            Attack *atk =
                    Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                                   10, 0.4, -0.4);
            atk->setAtkDuration(airPlungeGroundParticle->getSpriteLengthFromTo(0, 2));
        }
    }

    // Push Atk
    if (isNewestFrame(6, KQ_AIR_PLUNGE)) {
        const int N = 20;
        double atkPolyPts[N][2];
        approxEllipse(atkPolyPts, N,
                      26, -8, 16, 50);
        Attack *atk =
                Global::currentWorld->addKQAtk(this, atkPolyPts, N,
                                               10, 0.4, -0.4);
        atk->setShouldRemove([](Attack *self, void *fParams) {
            return (!Particle::isActive(PARTICLE_KQ_AIR_PLUNGE));
        }, nullptr);
    }
}

void Keqing::onGameFrame() {
    LivingEntity::onGameFrame();

    if (!isLocked) {
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

void Keqing::setFacingEast(bool facingEast_) {
    if (isFacingEast() != facingEast_) {
        if (!isInAir()) {
            if (!isSpriteAnimated(KQ_RUN_START) &&
                !isSpriteAnimated(KQ_RUN) &&
                !isSpriteAnimated(KQ_RUN_TURN)) {
                setSpriteAnimated(true, KQ_WALK_TURN);
            } else {
                setSpriteAnimated(true, KQ_RUN_TURN);
            }
        }
    }
    Entity::setFacingEast(facingEast_);
}

bool Keqing::canDoAction(int spriteCode) {
    int spriteCodeToSkip[KQ_ENUM_N];
    std::fill(spriteCodeToSkip, spriteCodeToSkip + KQ_ENUM_N, 0);

    switch (spriteCode) {
        case KQ_WALK:
            spriteCodeToSkip[KQ_RUN] = 2;
            break;

        case KQ_SKILL:
        KqSkillCond:
            if (Particle::isActive(PARTICLE_KQ_SKILL_SPAWN)) return false;
            if (!Particle::isActive(PARTICLE_HUD_SKILL_CIRCLE)) return false;
            break;

        case KQ_SKILL_FLIP:
            spriteCodeToSkip[KQ_AIR_DOUBLE_JUMP] = 2;
            spriteCodeToSkip[KQ_AIR_NATK] = 2;
            spriteCodeToSkip[KQ_AIR_UP_NATK] = 2;
            spriteCodeToSkip[KQ_AIR_DASH] = 2;
            break;

        case KQ_BURST:
            if (!Particle::isActive(PARTICLE_HUD_BURST_CIRCLE)) return false;
            break;

        case KQ_AIR_DOUBLE_JUMP:
            if (jumpPressTime == Global::pressedTime[KEY_SPACE]) return false;
            if (airDoubleJumped) return false;
            spriteCodeToSkip[KQ_AIR_NATK] = 1;
            spriteCodeToSkip[KQ_AIR_UP_NATK] = 1;
            break;

        case KQ_AIR_DASH:
            if (airDashed) return false;
            break;

        case KQ_AIR_SKILL_CLONE:
            if (airASkillCloned) return false;
            break;

        case KQ_AIR_SKILL:
            goto KqSkillCond;
            break;

        default:
            break;
    }

    // Already Animated
    if (isSpriteAnimated(spriteCode)) return false;

    // Non Cancellable Skills
    if (spriteCode != hurtSpriteCode) {
        if (isSpriteAnimated(KQ_RUN_NATK)) return false;
        if (isSpriteAnimated(KQ_SKILL_FLIP)) return false;
        if (isSpriteAnimated(KQ_SKILL_FLIP_LAND)) return false;
        if (isSpriteAnimated(KQ_SKILL_CLONE)) return false;
        if (isSpriteAnimated(KQ_SKILL)) return false;
        if (isSpriteAnimated(KQ_SKILL_AIMING)) return false;
        if (isSpriteAnimated(KQ_SKILL_SLASH)) return false;
        if (isSpriteAnimated(KQ_BURST)) return false;
        if (isSpriteAnimated(KQ_AIR_SKILL_CLONE)) return false;
        if (isSpriteAnimated(KQ_AIR_SKILL)) return false;
        if (isSpriteAnimated(KQ_AIR_SKILL_AIMING)) return false;
        if (isSpriteAnimated(KQ_AIR_SKILL_SLASH)) return false;
        if (isSpriteAnimated(KQ_AIR_PLUNGE)) return false;
    }

    // Superior Priority
    for (int i = spriteCode + 1; i < KQ_ENUM_N; i++) {
        if (spriteCodeToSkip[i] > 0) {
            if (spriteCodeToSkip[i] > 1) {
                setSpriteAnimated(false, i);
            }
            continue;
        }
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
            if (isInAir()) {
                spriteCode = KQ_AIR_SKILL;
            } else {
                spriteCode = KQ_SKILL;
            }
        } else {
            if (isInAir()) {
                spriteCode = KQ_AIR_SKILL_SLASH;
            } else {
                spriteCode = KQ_SKILL_SLASH;
            }
        }

    } else if (isKeyPressed(KEY_A)) {
        if (isKeyPressed(KEY_S)) {
            if (isInAir()) {
                spriteCode = KQ_AIR_SKILL_CLONE;
            } else {
                spriteCode = KQ_SKILL_CLONE;
            }
        } else if (isDirectionalKeyPressed()) {
            spriteCode = KQ_SKILL_FLIP;
        }

    } else if (isKeyPressed(KEY_SHIFT)) {
        if (isInAir()) {
            spriteCode = KQ_AIR_DASH;
        } else {
            if (isDirectionalKeyPressed()) {
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
        } else if (isSpriteAnimated(KQ_RUN_START) ||
                   isSpriteAnimated(KQ_RUN)) {
            spriteCode = KQ_RUN_NATK;

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

    // Ground
    if (isSpriteAnimated(KQ_NATK)) this->NAtk();
    if (isSpriteAnimated(KQ_CATK)) this->CAtk();
    if (isSpriteAnimated(KQ_UP_NATK)) this->upNAtk();
    if (isSpriteAnimated(KQ_UP_CATK)) this->upCAtk();
    if (isSpriteAnimated(KQ_DASH)) this->dash();
    if (isSpriteAnimated(KQ_RUN_NATK)) this->runNAtk();
    if (isSpriteAnimated(KQ_SKILL_FLIP) ||
        isSpriteAnimated(KQ_SKILL_FLIP_LAND))
        this->ASkillFlip();
    if (isSpriteAnimated(KQ_SKILL_CLONE)) this->ASkillClone();
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
    if (isSpriteAnimated(KQ_AIR_DASH)) this->airDash();
    if (isSpriteAnimated(KQ_AIR_SKILL_CLONE)) this->airASkillClone();
    if (isSpriteAnimated(KQ_AIR_SKILL)) this->airESkill();
    if (isSpriteAnimated(KQ_AIR_SKILL_AIMING)) this->airESkillAiming();
    if (isSpriteAnimated(KQ_AIR_SKILL_SLASH)) this->airESkillSlash();
    if (isSpriteAnimated(KQ_AIR_PLUNGE)) this->airPlunge();
}

void Keqing::kqLock(bool shouldLock) {
    isLocked = shouldLock;
    if (shouldLock) {
        for (int i = KQ_IDLE + 1; i < KQ_ENUM_N; i++) {
            setSpriteAnimated(false, i);
        }
        setSpriteAnimated(true, KQ_IDLE);
    }
}
