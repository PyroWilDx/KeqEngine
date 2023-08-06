//
// Created by pyrowildx on 13/05/23.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <SDL2/SDL.h>
#include "EntityBase/Entity.hpp"
#include "EntityBase/AnimatedEntity.hpp"
#include "Entity/Particle.hpp"

#define SCREEN_BASE_WIDTH 1280
#define SCREEN_BASE_HEIGHT 720

#define HUD_SB_CIRCLE_M 4.0
#define HUB_SB_ICON_M (HUD_SB_CIRCLE_M * 0.74)

#define KEY_PRESS_SHORT_DURATION 100

#define COLOR_WHITE 255, 255, 255
#define COLOR_WHITE_FULL COLOR_WHITE, 255
#define COLOR_BLACK 0, 0, 0
#define COLOR_BLACK_FULL COLOR_BLACK, 255
#define COLOR_RED 255, 0, 0
#define COLOR_RED_FULL COLOR_RED, 255
#define COLOR_GREEN 0, 255, 0
#define COLOR_GREEN_FULL COLOR_GREEN, 255
#define COLOR_BLUE 0, 0, 255
#define COLOR_BLUE_FULL COLOR_BLUE, 255

void myAssert(bool expr, const char *msg = nullptr, const char *err = nullptr);

int roundToInt(double value);

Uint32 cvStringToUint32(std::string &valStr);

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

void Uint32RGBAToUint8RGBA(Uint32 rgba, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);

void RGBToHSV(Uint8 r, Uint8 g, Uint8 b,
              double *pH, double *pS, double *pV);

void HSVToRGB(double h, double s, double v,
              Uint8 *pR, Uint8 *pG, Uint8 *pB);

#endif
