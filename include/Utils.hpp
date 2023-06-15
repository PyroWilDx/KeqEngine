//
// Created by pyrowildx on 13/05/23.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL2/SDL.h>
#include "Entity.hpp"
#include "AnimatedEntity.hpp"
#include "Particle.hpp"

//Screen dimension constants
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define BACKGROUND_WIDTH 3000

#define MIN_X (-40)
#define MAX_X (BACKGROUND_WIDTH - 20)

#define HUD_SB_CIRCLE_M 4.0f
#define HUB_SB_ICON_M (HUD_SB_CIRCLE_M * 0.74f)

void myAssert(bool expr, const char *msg, const char *err);

bool isNewestFrame(Sprite *sprite, int x);

bool isSameSpriteCode(Sprite *sprite, int spriteCode);

inline int getTime() { return ((int) SDL_GetTicks()); }

void RGBtoHSV(Uint8 r, Uint8 g, Uint8 b,
              float *pH, float *pS, float *pV);

void HSVtoRGB(float h, float s, float v,
              Uint8 *pR, Uint8 *pG, Uint8 *pB);

#endif
