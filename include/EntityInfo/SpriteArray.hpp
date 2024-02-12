//
// Created by pyrowildx on 16/07/2023.
//

#ifndef SPRITE_HPP
#define SPRITE_HPP

#include <SDL2/SDL.h>

class SpriteArray;

typedef struct Sprite {
    int sCode;
    const char *imgPath;
    SDL_Texture *sTexture;
    bool sAnimated;
    bool sPaused;
    int sFrameW;
    int sFrameH;
    int sFrameN;
    int sCurrentFrame;
    int *sFrameLengths;
    int sTimer;
    Sprite *sNext;
} Sprite;

class SpriteArray {

public:
    explicit SpriteArray(int spriteArrayLength);

    ~SpriteArray();

    Sprite &operator[](int index) {
        return spriteArray[index];
    }

    [[nodiscard]] inline int getSpriteArrayLength() const { return spriteArrayLength; }

private:
    int spriteArrayLength;
    Sprite *spriteArray;

};

#endif
