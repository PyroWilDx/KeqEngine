//
// Created by pyrowildx on 13/05/23.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL2/SDL.h>
#include "Entity.hpp"
#include "AnimatedEntity.hpp"

//Screen dimension constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define DEFAULT_Y 520

#define BACKGROUND_WIDTH 3000

#define MIN_X (-40)
#define MIN_Z (-10000)
#define MAX_Z 10000

const int MAX_X = BACKGROUND_WIDTH - 20;

void myAssert(bool expr, const char *msg, const char *err);

bool isNewestFrame(Sprite *sprite, int x);

bool isSameSpriteCode(Sprite *sprite, int spriteCode);

#endif
