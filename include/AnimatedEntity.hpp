//
// Created by pyrowildx on 30/05/23.
//

#ifndef ANIMATEDENTITY_HPP
#define ANIMATEDENTITY_HPP

#include "Entity.hpp"
#include "WindowRenderer.hpp"

typedef struct Sprite {
    int sCode;
    SDL_Texture *sTexture;
    bool sAnimated;
    int sFrameW;
    int sFrameH;
    int sFrameN;
    int sCurrentFrame;
    int *sFrameLengths;
    int sTimer;
    Sprite *sNext;
} Sprite;


class AnimatedEntity : public Entity {

public:
    explicit AnimatedEntity(int spriteArrayLength);

    ~AnimatedEntity() override;

    void setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) override;

    void initSprite(int spriteCode, const char *imgPath,
                    int spriteFrameW, int spriteFrameH, int spriteFrameN,
                    int spriteFrameLength = 0);

    virtual void setSpriteAnimated(int spriteCode, bool animated);

    void setSpriteFrameLengthFromTo(int spriteCode, int spriteFrameLength,
                                    int startFrame = -2, int endFrame = -2);

    void setSpriteFrameLengths(int spriteCode, const int *spriteFrameLengths);

    void setSpriteNext(int spriteCode, int nextSpriteCode);

    bool isSpriteAnimated(int spriteCode);

    bool isFrameAt(int spriteCode, int frameIndex);

    bool isNewestFrame(int spriteCode, int frameIndex);

    bool isFrameBetween(int spriteCode, int startFrame, int endFrame);

    bool willFrameFinish(int spriteCode, int frameIndex);

    bool isCurrentSprite(int spriteCode);

    void goToFrame(int spriteCode, int frameIndex);

    void goToNextFrame(int spriteCode);

    void stopOnFrame(int spriteCode, int frameIndex = -1);

    void resetSprite(int spriteCode);

    void delaySprite(int spriteCode, int ms);

    void animateSprite();

//    virtual void updateSprite(int spriteCode) = 0;

protected:
    int spriteArrayLength;
    Sprite *spriteArray;
    int currSpriteCode;

private:

};

#endif
