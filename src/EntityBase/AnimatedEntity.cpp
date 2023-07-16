//
// Created by pyrowildx on 30/05/23.
//

#include "EntityBase/AnimatedEntity.hpp"
#include "Utils/Global.hpp"

AnimatedEntity::AnimatedEntity(int spriteArrayLength)
        : Entity() {
    this->spriteArrayLength = spriteArrayLength;
    spriteArray = new Sprite[spriteArrayLength];
    spriteArray[spriteArrayLength].sAnimated = false;
    currentSprite = &spriteArray[0];
}

AnimatedEntity::~AnimatedEntity() {
    imgTexture = nullptr;
}

void AnimatedEntity::setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    for (int i = 0; i < spriteArrayLength; i++) {
        SDL_Texture *currTexture = spriteArray[i].sTexture;
        SDL_SetTextureColorMod(currTexture, r, g, b);
        SDL_SetTextureAlphaMod(currTexture, a);
    }
}

void AnimatedEntity::initSprite(int spriteCode, const char *imgPath,
                                int spriteFrameW, int spriteFrameH, int spriteFrameN,
                                int spriteFrameLength) {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    spriteArray[spriteCode] =
            {spriteCode, imgPath, gWindow->loadTexture(imgPath), false,
             spriteFrameW, spriteFrameH, spriteFrameN, 0,
             new int[spriteFrameN], 0, nullptr};

    if (spriteFrameLength != 0) {
        setSpriteFrameLengthFromTo(spriteCode, spriteFrameLength);
    }
}

void AnimatedEntity::setSpriteAnimated(int spriteCode, bool animated) {
    spriteArray[spriteCode].sAnimated = animated;
    if (animated) {
        currentSprite = &spriteArray[spriteCode];
    }
    if (!animated) {
        resetSprite(spriteCode);
    }
}

void AnimatedEntity::setSpriteFrameLengthFromTo(int spriteCode, int spriteFrameLength,
                                                int startFrame, int endFrame) {
    Sprite *sprite = &spriteArray[spriteCode];

    if ((startFrame == -2) && (endFrame == -2)) {
        startFrame = 0;
        endFrame = sprite->sFrameN - 1;
    } else if (endFrame == -2) {
        endFrame = startFrame;
    }

    if (endFrame == -1) {
        endFrame = sprite->sFrameN - 1;
    }

    for (int i = startFrame; i <= endFrame; i++) {
        sprite->sFrameLengths[i] = spriteFrameLength;
    }
}

void AnimatedEntity::setSpriteFrameLengths(int spriteCode, const int *spriteFrameLengths) {
    Sprite *sprite = &spriteArray[spriteCode];
    for (int i = 0; i < sprite->sFrameN; i++) {
        sprite->sFrameLengths[i] = spriteFrameLengths[i];
    }
}

void AnimatedEntity::setSpriteNext(int spriteCode, int nextSpriteCode) {
    spriteArray[spriteCode].sNext = &spriteArray[nextSpriteCode];
}

bool AnimatedEntity::isSpriteAnimated(int spriteCode) {
    return (spriteArray[spriteCode].sAnimated);
}

bool AnimatedEntity::isFrameAt(int spriteCode, int frameIndex) {
    if (frameIndex == -1) frameIndex = spriteArray[spriteCode].sFrameN - 1;
    return (spriteArray[spriteCode].sCurrentFrame == frameIndex);
}

bool AnimatedEntity::isNewestFrame(int spriteCode, int frameIndex) {
    if (frameIndex == -1) frameIndex = spriteArray[spriteCode].sFrameN - 1;
    return (isFrameAt(spriteCode, frameIndex) &&
            spriteArray[spriteCode].sTimer == 0);
}

bool AnimatedEntity::isFrameBetween(int spriteCode, int startFrame, int endFrame) {
    Sprite *sprite = &spriteArray[spriteCode];

    if (endFrame == -1) endFrame = sprite->sFrameN - 1;

    return (sprite->sCurrentFrame >= startFrame &&
            sprite->sCurrentFrame <= endFrame);
}

bool AnimatedEntity::willFrameFinish(int spriteCode, int frameIndex) {
    if (frameIndex == -1) frameIndex = spriteArray[spriteCode].sFrameN - 1;
    Sprite *sprite = &spriteArray[spriteCode];
    return (isFrameAt(spriteCode, frameIndex) &&
            sprite->sTimer + Global::dt > sprite->sFrameLengths[frameIndex]);
}

bool AnimatedEntity::isCurrentSprite(int spriteCode) {
    return (spriteCode == currentSprite->sCode);
}

void AnimatedEntity::goToFrame(int spriteCode, int frameIndex) {
    if (frameIndex == -1) {
        frameIndex = spriteArray[spriteCode].sFrameN - 1;
    }
    spriteArray[spriteCode].sTimer = 0;
    spriteArray[spriteCode].sCurrentFrame = frameIndex;
}

void AnimatedEntity::goToNextFrame(int spriteCode) {
    goToFrame(spriteCode, spriteArray[spriteCode].sCurrentFrame + 1);
    spriteArray[spriteCode].sTimer = 0;
}

void AnimatedEntity::stopOnFrame(int spriteCode, int frameIndex) {
    if (frameIndex == -1) {
        frameIndex = spriteArray[spriteCode].sFrameN - 1;
    }
    spriteArray[spriteCode].sFrameLengths[frameIndex] = INT32_MAX;
}

void AnimatedEntity::pauseSprite(int spriteCode, bool pause) {
    spriteArray[spriteCode].sAnimated = !pause;
}

void AnimatedEntity::resetSprite(int spriteCode) {
    Sprite *sprite = &spriteArray[spriteCode];
    sprite->sCurrentFrame = 0;
    sprite->sTimer = 0;
}

void AnimatedEntity::delaySprite(int spriteCode, int ms) {
    spriteArray[spriteCode].sTimer -= ms;
}

void AnimatedEntity::animateSprite() {
    Sprite *lastAnimatedSprite = nullptr;
    for (int i = 0; i < spriteArrayLength; i++) {
        Sprite *sprite = &spriteArray[i];
        if (sprite->sAnimated) {
            sprite->sTimer += Global::dt;

            if (sprite->sTimer < 0) continue;

            int currentFrame = sprite->sCurrentFrame;
            if (sprite->sTimer > sprite->sFrameLengths[currentFrame]) {
                sprite->sCurrentFrame++;
                if (sprite->sCurrentFrame == sprite->sFrameN) {
                    setSpriteAnimated(sprite->sCode, false);
                    if (sprite->sNext != nullptr) {
                        setSpriteAnimated(sprite->sNext->sCode, true);
                        lastAnimatedSprite = sprite->sNext;
                    }
                    continue;
                }
                sprite->sTimer = 0;
            }
            lastAnimatedSprite = sprite;
        }
    }

    if (lastAnimatedSprite != nullptr) {
        imgTexture = lastAnimatedSprite->sTexture;
        imgFrame.x = lastAnimatedSprite->sCurrentFrame * lastAnimatedSprite->sFrameW;
        imgFrame.w = lastAnimatedSprite->sFrameW;
        imgFrame.h = lastAnimatedSprite->sFrameH;
        currentSprite = lastAnimatedSprite;
    }
}
