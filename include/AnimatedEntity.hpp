//
// Created by pyrowildx on 30/05/23.
//

#ifndef ANIMATEDENTITY_HPP
#define ANIMATEDENTITY_HPP

#include "Entity.hpp"
#include "WindowRenderer.hpp"

typedef struct SpriteTexture {
    bool animated;
    bool oneTime;
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
    SpriteTexture *next;
} SpriteTexture;

class AnimatedEntity : public Entity {

public:
    AnimatedEntity(bool hasShadow, int n);

    void setTextureAnimated(int code, bool animated, bool reset = true);

    void animate(int dt);

    void destroy() override;

    inline SpriteTexture *getSpriteArray() { return spriteArray; }

protected:
    int n;
    SpriteTexture *spriteArray;

private:

};

#endif
