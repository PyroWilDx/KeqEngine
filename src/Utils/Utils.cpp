//
// Created by pyrowildx on 13/05/23.
//

#include <cassert>
#include <algorithm>
#include "Utils/Utils.hpp"
#include "Utils/Global.hpp"

#define DOUBLE_EPSILON 0.001
#define KEY_PRESS_SHORT_DURATION 100

void myAssert(bool expr, const char *msg, const char *err) {
    if (!expr) {
        SDL_Log("Message : %s\n", msg);
        SDL_Log("Error : %s\n", err);
        assert(expr);
    }
}

int roundToInt(double x) {
    return ((int) (x));
}

bool areDoubleEpsilonEqual(double a, double b) {
    return (abs(a - b) < DOUBLE_EPSILON);
}

int getTime() {
    return ((int) SDL_GetTicks());
}

void handleTime() {
    int currentTime = getTime();
    Global::dt = currentTime - Global::currentTime;
    Global::currentTime = currentTime;
}

void getScreenXYCoeff(double *pXCoeff, double *pYCoeff) {
    *pXCoeff = (double) Global::renderWindowWidth / SCREEN_BASE_WIDTH;
    *pYCoeff = (double) Global::renderWindowHeight / SCREEN_BASE_HEIGHT;
}

void shiftXYScreen(int *pX, int *pY) {
    *pX += (Global::windowWidth - Global::renderWindowWidth) / 2;
    *pY += (Global::windowHeight - Global::renderWindowHeight) / 2;
}

int getSDLKeyRelation(int SDLKey, bool isKeyboard) {
    if (!isKeyboard) {
        if (SDLKey == SDL_BUTTON_LEFT) return KEY_MOUSE_LEFT;
        if (SDLKey == SDL_BUTTON_RIGHT) return KEY_MOUSE_RIGHT;

        return -1;
    }

    if (SDLKey == SDLK_z) return KEY_Z;
    if (SDLKey == SDLK_q) return KEY_Q;
    if (SDLKey == SDLK_s) return KEY_S;
    if (SDLKey == SDLK_d) return KEY_D;
    if (SDLKey == SDLK_e) return KEY_E;
    if (SDLKey == SDLK_r) return KEY_R;
    if (SDLKey == SDLK_SPACE) return KEY_SPACE;
    if (SDLKey == SDLK_LSHIFT) return KEY_SHIFT;

    return -1;
}

int updatePressedKeys(int SDLKey, bool keyPressed, bool isKeyboard) {
    int key = getSDLKeyRelation(SDLKey, isKeyboard);
    if (key != -1) {
        if (Global::pressedKeys[key] != keyPressed) {
            Global::pressedKeys[key] = keyPressed;
            if (keyPressed) {
                Global::lastPressedTime[key] = Global::pressedTime[key];
                Global::pressedTime[key] = Global::currentTime;
            }
        }
    }
    return key;
}

bool isKeyPressed(int key) {
    return (Global::pressedKeys[key]);
}

bool isKeyPressedRecent(int key) {
    return ((Global::currentTime - Global::pressedTime[key]) < KEY_PRESS_SHORT_DURATION);
}

bool isKeyPressedShort(int key) {
    return (isKeyPressed(key) && isKeyPressedRecent(key));
}

bool isKeyPressedLong(int key) {
    return (isKeyPressed(key) && !isKeyPressedRecent(key));
}

bool isMouseLeftRecent() {
    return (isKeyPressedRecent(KEY_MOUSE_LEFT));
}

bool isMouseLeftShort() {
    return (isKeyPressedShort(KEY_MOUSE_LEFT));
}

bool isMouseLeftLong() {
    return (isKeyPressedLong(KEY_MOUSE_LEFT));
}

bool isKeyDoublePressed(int key) {
    if (!isKeyPressed(key)) return false;
    return (Global::pressedTime[key] - Global::lastPressedTime[key] < 200);
}

void RGBtoHSV(Uint8 r, Uint8 g, Uint8 b,
              double *pH, double *pS, double *pV) {
    double fr = (double) r / 255.0;
    double fg = (double) g / 255.0;
    double fb = (double) b / 255.0;

    double maxVal = std::max(fr, std::max(fg, fb));
    double minVal = std::min(fr, std::min(fg, fb));

    double h, s, v;

    v = maxVal;

    if (maxVal != 0) {
        s = (maxVal - minVal) / maxVal;
    } else {
        s = 0;
    }

    if (s == 0)
        h = 0;
    else {
        double delta = maxVal - minVal;

        if (maxVal == fr) {
            h = (fg - fb) / delta;
        } else if (maxVal == fg) {
            h = 2.0 + (fb - fr) / delta;
        } else {
            h = 4.0 + (fr - fg) / delta;
        }

        h *= 60.0;

        if (h < 0) h += 360.0;
    }

    if (pH != nullptr) *pH = h;
    if (pS != nullptr) *pS = s;
    if (pV != nullptr) *pV = v;
}

void HSVtoRGB(double h, double s, double v,
              Uint8 *pR, Uint8 *pG, Uint8 *pB) {
    if (s == 0) {
        *pR = *pG = *pB = (Uint8) (v * 255.0);
        return;
    }

    int r, g, b;

    h /= 60.0;
    int i = (int) h;
    double f = h - (double) i;
    double p = v * (1.0 - s);
    double q = v * (1.0 - s * f);
    double t = v * (1.0 - s * (1.0 - f));

    switch (i) {
        case 0:
            r = (Uint8) (v * 255.0);
            g = (Uint8) (t * 255.0);
            b = (Uint8) (p * 255.0);
            break;
        case 1:
            r = (Uint8) (q * 255.0);
            g = (Uint8) (v * 255.0);
            b = (Uint8) (p * 255.0);
            break;
        case 2:
            r = (Uint8) (p * 255.0);
            g = (Uint8) (v * 255.0);
            b = (Uint8) (t * 255.0);
            break;
        case 3:
            r = (Uint8) (p * 255.0);
            g = (Uint8) (q * 255.0);
            b = (Uint8) (v * 255.0);
            break;
        case 4:
            r = (Uint8) (t * 255.0);
            g = (Uint8) (p * 255.0);
            b = (Uint8) (v * 255.0);
            break;
        default:
            r = (Uint8) (v * 255.0);
            g = (Uint8) (p * 255.0);
            b = (Uint8) (q * 255.0);
            break;
    }

    *pR = r;
    *pG = g;
    *pB = b;
}
