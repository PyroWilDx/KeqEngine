//
// Created by pyrowildx on 30/05/23.
//

#include "AnimatedEntity.hpp"
#include "Keqing.hpp"

AnimatedEntity::AnimatedEntity(int n)
        : Entity(0, 0, 0, 0, nullptr) {
    this->n = n;
    spriteArray = new Sprite[n + 1];
    spriteArray[n].animated = false;
}

AnimatedEntity::~AnimatedEntity() {
    texture = nullptr;
}

void AnimatedEntity::setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    for (int i = 0; i < n; i++) {
        SDL_Texture *currTexture = spriteArray[i].texture;
        SDL_SetTextureColorMod(currTexture, r, g, b);
        SDL_SetTextureAlphaMod(currTexture, a);
    }
}

void AnimatedEntity::setSpriteAnimated(int spriteCode, bool animated, bool reset) {
    spriteArray[spriteCode].animated = animated;
    if (!animated) {
        if (reset) {
            spriteArray[spriteCode].frameX = 0;
        }
        spriteArray[spriteCode].accumulatedTime = 0;
    }
}

void AnimatedEntity::animate(int dt) {
    Sprite *lastAnimatedSprite = nullptr;
    for (int i = 0; i < n + 1; i++) {
        Sprite *currentSprite = &spriteArray[i];
        if (currentSprite->animated) {
            currentSprite->accumulatedTime += dt;
            if (currentSprite->accumulatedTime > currentSprite->frameDuration) {
                currentSprite->frameX += currentSprite->width;
                if (currentSprite->frameX == currentSprite->maxWidth) {
                    setSpriteAnimated(i, false);
                    if (currentSprite->next != nullptr) {
                        setSpriteAnimated(currentSprite->next->code, true);
                        lastAnimatedSprite = currentSprite->next;
                    }
                    continue;
                }
                currentSprite->accumulatedTime = 0;
            }
            lastAnimatedSprite = currentSprite;
        }
    }
    if (lastAnimatedSprite != nullptr) {
        texture = lastAnimatedSprite->texture;
        frame.x = lastAnimatedSprite->frameX;
        frame.w = lastAnimatedSprite->width;
        frame.h = lastAnimatedSprite->height;
        xShift = lastAnimatedSprite->xShift;
        yShift = lastAnimatedSprite->yShift;
        xShiftR = lastAnimatedSprite->xShiftR;
    }
}

void AnimatedEntity::moveSpriteFrameX(int spriteCode, int x) {
    spriteArray[spriteCode].frameX = x;
}

void AnimatedEntity::forceSprite(int spriteCode, int newMaxWidth,
                                 int newFrameDuration, int startX) {
    spriteArray[n] = spriteArray[spriteCode];
    Sprite *forcedSprite = &spriteArray[n];
    forcedSprite->animated = true;
    forcedSprite->maxWidth = newMaxWidth;
    forcedSprite->frameDuration = newFrameDuration;
    forcedSprite->frameX = startX;
}

void AnimatedEntity::removeForcedSprite() {
    spriteArray[n].animated = false;
}

void AnimatedEntity::delay(int spriteCode, int ms) {
    Sprite *sprite = &spriteArray[spriteCode];
    sprite->frameX -= (ms / sprite->frameDuration) * sprite->width;
}

void AnimatedEntity::reset(int spriteCode) {
    Sprite *sprite = &spriteArray[spriteCode];
    sprite->frameX = 0;
    sprite->accumulatedTime = 0;
}

int AnimatedEntity::getTotalDuration(int spriteCode) {
    Sprite *sprite = &spriteArray[spriteCode];
    return (sprite->frameDuration * (sprite->maxWidth / sprite->width));
}
