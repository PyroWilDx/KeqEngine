//
// Created by pyrowildx on 13/05/23.
//

#include <cassert>
#include <cstdio>
#include "Utils.hpp"

void myAssert(bool expr, const char *msg, const char *err) {
    if (!expr) {
        printf("Message : %s\n", msg);
        printf("Error : %s\n", err);
        assert(expr);
    }
}

bool isNewestFrame(Sprite *sprite, int x) {
    return (sprite->currentFrameX == x && sprite->accumulatedTime == 0);
}

bool isSameSpriteCode(Sprite *sprite, int spriteCode) {
    return (sprite->code == spriteCode);
}
