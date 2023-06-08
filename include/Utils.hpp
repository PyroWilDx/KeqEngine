//
// Created by pyrowildx on 13/05/23.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL2/SDL.h>
#include "Entity.hpp"
#include "AnimatedEntity.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const int DEFAULT_Y = SCREEN_HEIGHT - 200;

const int MIN_X = -40;
const int MAX_X = SCREEN_WIDTH + 90;
const float Z_MULTIPLIER = 0.6f;
const int MIN_Z = -10000;
const int MAX_Z = 10000;

void myAssert(bool expr, const char *msg, const char *err);

bool isNewestFrame(Sprite *sprite, int wantedFrameX);

#endif
