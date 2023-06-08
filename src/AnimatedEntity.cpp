//
// Created by pyrowildx on 30/05/23.
//

#include "AnimatedEntity.hpp"

AnimatedEntity::AnimatedEntity(bool hasShadow, int n)
        : Entity(0, 0, 0, 0, 0, hasShadow, nullptr) {
    this->n = n;
    spriteArray = new Sprite[n + 1];
    spriteArray[n].animated = false;
}

void AnimatedEntity::setTextureAnimated(int spriteCode, bool animated, bool reset) {
    spriteArray[spriteCode].animated = animated;
    if (!animated) {
        if (reset) {
            spriteArray[spriteCode].currentFrameX = 0;
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
                currentSprite->currentFrameX += currentSprite->width;
                // if (currentSprite->currentFrameX >= currentSprite->maxWidth) {
                if (currentSprite->currentFrameX == currentSprite->maxWidth) {
                    setTextureAnimated(i, false);
                    if (currentSprite->next != nullptr) {
                        currentSprite->next->animated = true;
                    }
                }
                currentSprite->accumulatedTime = 0;
            }
            lastAnimatedSprite = currentSprite;
        }
    }
    if (lastAnimatedSprite != nullptr) {
        texture = lastAnimatedSprite->texture;
        frame.x = lastAnimatedSprite->currentFrameX;
        frame.w = lastAnimatedSprite->width;
        frame.h = lastAnimatedSprite->height;
        xShift = lastAnimatedSprite->xShift;
        yShift = lastAnimatedSprite->yShift;
        xShiftR = lastAnimatedSprite->xShiftR;
    }
}

void AnimatedEntity::forceSprite(int spriteCode, int newMaxWidth,
                                 int newFrameDuration, int startX) {
    spriteArray[n] = spriteArray[spriteCode];
    Sprite *forcedSprite = &spriteArray[n];
    forcedSprite->animated = true;
    forcedSprite->maxWidth = newMaxWidth;
    forcedSprite->frameDuration = newFrameDuration;
    forcedSprite->currentFrameX = startX;
}

void AnimatedEntity::removeForcedSprite() {
    spriteArray[n].animated = false;
}

void AnimatedEntity::destroy() {
    for (int i = 0; i < n; i++) {
        SDL_DestroyTexture(spriteArray[i].texture);
    }
    delete[] spriteArray;
}
