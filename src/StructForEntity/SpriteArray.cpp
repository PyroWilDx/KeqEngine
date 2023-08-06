//
// Created by pyrow on 16/07/2023.
//

#include "StructForEntity/SpriteArray.hpp"

SpriteArray::SpriteArray(int spriteArrayLength) {
    this->spriteArrayLength = spriteArrayLength;
    spriteArray = new Sprite[spriteArrayLength];
    for (int i = 0; i < spriteArrayLength; i++) {
        Sprite *sprite = &spriteArray[i];
        sprite->sCode = i;
        sprite->imgPath = nullptr;
        sprite->sTexture = nullptr;
        sprite->sAnimated = false;
        sprite->sFrameW = 0;
        sprite->sFrameH = 0;
        sprite->sFrameN = 0;
        sprite->sCurrentFrame = 0;
        sprite->sFrameLengths = nullptr;
        sprite->sTimer = 0;
        sprite->sNext = nullptr;
    }
}

SpriteArray::~SpriteArray() {
    delete[] spriteArray;
}
