//
// Created by pyrow on 16/07/2023.
//

#include "Utils/SpriteArray.hpp"

SpriteArray::SpriteArray(int spriteArrayLength) {
    this->spriteArrayLength = spriteArrayLength;
    spriteArray = new Sprite[spriteArrayLength];
}
