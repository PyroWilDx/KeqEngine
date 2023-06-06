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
    int width;
    int height;
    int maxWidth;
    int currentFrameX;
    int timeBetweenFrames;
    int accumulatedTime;
} SpriteTexture;

class AnimatedEntity : public Entity {

public:
    AnimatedEntity(int w, int h, bool hasShadow, int n);

    void setTextureAnimated(int code, bool animated);

    bool animate(int dt);

    void destroy() override;

protected:
    int n;
    SpriteTexture *spriteArray;

private:

};

#endif
