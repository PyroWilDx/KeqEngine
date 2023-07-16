//
// Created by pyrowildx on 13/05/23.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <SDL2/SDL.h>
#include "EntityBase/Entity.hpp"
#include "EntityBase/AnimatedEntity.hpp"
#include "Entity/Particle.hpp"

//Screen dimension constants
#define SCREEN_BASE_WIDTH 1280
#define SCREEN_BASE_HEIGHT 720

#define MIN_X (-40)
#define MAX_X (3000 - 20)

#define HUD_SB_CIRCLE_M 4.0
#define HUB_SB_ICON_M (HUD_SB_CIRCLE_M * 0.74)

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

int roundToInt(double value);

bool areDoubleEpsilonEqual(double a, double b);

int getTime();

void handleTime();

void getScreenXYCoeff(double *pXCoeff, double *pYCoeff);

void shiftXYFromScreenPosition(int *pX, int *pY);

void getMouseAbsoluteXY(int *pMouseX, int *pMouseY);

int getSDLKeyRelation(int SDLKey, bool isKeyboard);

int updatePressedKeys(int SDLKey, bool keyPressed, bool isKeyboard);

bool isKeyPressed(int key);

bool isKeyPressedRecent(int key);

bool isKeyPressedShort(int key);

bool isKeyPressedLong(int key);

bool isMouseLeftRecent();

bool isMouseLeftShort();

bool isMouseLeftLong();

bool isKeyDoublePressed(int key);

void RGBtoHSV(Uint8 r, Uint8 g, Uint8 b,
              double *pH, double *pS, double *pV);

void HSVtoRGB(double h, double s, double v,
              Uint8 *pR, Uint8 *pG, Uint8 *pB);

#endif
