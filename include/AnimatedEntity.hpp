//
// Created by pyrowildx on 30/05/23.
//

#ifndef ANIMATEDENTITY_HPP
#define ANIMATEDENTITY_HPP

#include "Entity.hpp"
#include "WindowRenderer.hpp"

typedef struct Sprite {
    int code;
    bool animated;
    SDL_Texture *texture;
    int xShift;
    int yShift;
    int xShiftR;
    int width;
    int height;
    int maxWidth;
    int frameDuration;
    int currentFrameX;
    int accumulatedTime;
    Sprite *next;
} Sprite;

class AnimatedEntity : public Entity {

public:
    AnimatedEntity(bool hasShadow, int n);

    void setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) override;

    void setSpriteAnimated(int spriteCode, bool animated, bool reset = true);

    void animate(int dt);

    void moveSpriteFrameX(int spriteCode, int x);

    void forceSprite(int spriteCode, int newMaxWidth,
                     int newFrameDuration, int startX);

    void removeForcedSprite();

    void destroy() override;

protected:
    int n;
    Sprite *spriteArray;

private:

};

#endif
