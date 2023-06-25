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
#define SCREEN_BASE_WIDTH 1280
#define SCREEN_BASE_HEIGHT 720

#define BACKGROUND_WIDTH 3000

#define MIN_X (-40)
#define MAX_X (BACKGROUND_WIDTH - 20)

#define HUD_SB_CIRCLE_M 4.0f
#define HUB_SB_ICON_M (HUD_SB_CIRCLE_M * 0.74f)

#define RGB_FULL 255, 255, 255
#define RGBA_FULL 255, 255, 255, 255

enum {
    KEY_MOUSE_LEFT,
    KEY_MOUSE_RIGHT,
    KEY_Z,
    KEY_Q,
    KEY_S,
    KEY_D,
    KEY_A,
    KEY_E,
    KEY_R,
    KEY_SPACE,
    KEY_SHIFT,
    KEY_ENUM_N
};

void myAssert(bool expr, const char *msg, const char *err);

int getTime();

int getSDLKeyRelation(int SDLKey, bool isKeyboard);

int updatePressedKeys(int SDLKey, bool isKeyPressed, bool isKeyboard);

bool isKeyPressedRecent(int key);

bool isMouseLeftRecent();

void RGBtoHSV(Uint8 r, Uint8 g, Uint8 b,
              float *pH, float *pS, float *pV);

void HSVtoRGB(float h, float s, float v,
              Uint8 *pR, Uint8 *pG, Uint8 *pB);

#endif
