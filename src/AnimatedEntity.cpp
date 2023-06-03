//
// Created by pyrowildx on 30/05/23.
//

#include "AnimatedEntity.hpp"

AnimatedEntity::AnimatedEntity(int w, int h, bool hasShadow, int n)
        : Entity(0, 0, 0, w, h, hasShadow, nullptr) {
    this->n = n;
    spriteArray = new SpriteTexture[n];
}

void AnimatedEntity::setTextureAnimated(int code, bool animated) {
    spriteArray[code].animated = animated;
    if (!animated) {
        spriteArray[code].currentFrameX = 0;
        spriteArray[code].accumulatedTime = 0;
    }
}

void AnimatedEntity::animate(int dt) {
    SpriteTexture *lastAnimatedSprite;
    for (int i = 0; i < n; i++) {
        SpriteTexture *currentSprite = &spriteArray[i];
        if (currentSprite->animated) {
            currentSprite->accumulatedTime += dt;
            if (currentSprite->accumulatedTime > currentSprite->timeBetweenFrames) {
                currentSprite->currentFrameX += frame.w;
                if (currentSprite->currentFrameX >= currentSprite->totalWidth) {
                    if (!currentSprite->oneTime) {
                        currentSprite->currentFrameX = 0;
                    } else {
                        setTextureAnimated(i, false);
                    }
                }
                currentSprite->accumulatedTime = 0;
            }
            lastAnimatedSprite = currentSprite;
        }
    }
    frame.x = lastAnimatedSprite->currentFrameX;
    texture = lastAnimatedSprite->texture;
}

void AnimatedEntity::destroy() {
    for (int i = 0; i < n; i++) {
        SDL_DestroyTexture(spriteArray[i].texture);
    }
    delete[] spriteArray;
}

