//
// Created by pyrowildx on 30/05/23.
//

#include "AnimatedEntity.hpp"

AnimatedEntity::AnimatedEntity(bool hasShadow, int n)
        : Entity(0, 0, 0, 0, 0, hasShadow, nullptr) {
    this->n = n;
    spriteArray = new SpriteTexture[n];
}

void AnimatedEntity::setTextureAnimated(int code, bool animated, bool reset) {
    spriteArray[code].animated = animated;
    if (!animated) {
        if (reset) {
            spriteArray[code].currentFrameX = 0;
        }
        spriteArray[code].accumulatedTime = 0;
    }
}

void AnimatedEntity::animate(int dt) {
    SpriteTexture *lastAnimatedSprite;
    for (int i = 0; i < n; i++) {
        SpriteTexture *currentSprite = &spriteArray[i];
        if (currentSprite->animated) {
            currentSprite->accumulatedTime += dt;
            if (currentSprite->accumulatedTime > currentSprite->frameDuration) {
                currentSprite->currentFrameX += currentSprite->width;
                if (currentSprite->currentFrameX >= currentSprite->maxWidth) {
                    if (!currentSprite->oneTime) {
                        currentSprite->currentFrameX = 0;
                    } else {
                        setTextureAnimated(i, false);
                        if (currentSprite->next != nullptr) {
                            currentSprite->next->animated = true;
                        }
                    }
                }
                currentSprite->accumulatedTime = 0;
            }
            lastAnimatedSprite = currentSprite;
        }
    }
    texture = lastAnimatedSprite->texture;
    frame.x = lastAnimatedSprite->currentFrameX;
    frame.w = lastAnimatedSprite->width;
    frame.h = lastAnimatedSprite->height;
    xShift = lastAnimatedSprite->xShift;
    yShift = lastAnimatedSprite->yShift;
    xShiftR = lastAnimatedSprite->xShiftR;
}

void AnimatedEntity::destroy() {
    for (int i = 0; i < n; i++) {
        SDL_DestroyTexture(spriteArray[i].texture);
    }
    delete[] spriteArray;
}
