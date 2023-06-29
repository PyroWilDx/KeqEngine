//
// Created by pyrowildx on 13/05/23.
//

#include <cassert>
#include <algorithm>
#include "Utils.hpp"
#include "Global.hpp"

void myAssert(bool expr, const char *msg, const char *err) {
    if (!expr) {
        SDL_Log("Message : %s\n", msg);
        SDL_Log("Error : %s\n", err);
        assert(expr);
    }
}

int roundToInt(double x) {
    return ((int) round(x));
}

int getTime() {
    return ((int) SDL_GetTicks());
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

int updatePressedKeys(int SDLKey, bool isKeyPressed, bool isKeyboard) {
    int key = getSDLKeyRelation(SDLKey, isKeyboard);
    if (key != -1) {
        Global::pressedKeys[key] = isKeyPressed;
        if (isKeyPressed) Global::pressedTime[key] = Global::currentTime;
    }
    return key;
}

bool isKeyPressedRecent(int key) {
    return ((Global::currentTime - Global::pressedTime[key]) < 100);
}

bool isMouseLeftRecent() {
    return isKeyPressedRecent(KEY_MOUSE_LEFT);
}

void RGBtoHSV(Uint8 r, Uint8 g, Uint8 b,
              double *pH, double *pS, double *pV) {
    double fr = (double) r / 255.0f;
    double fg = (double) g / 255.0f;
    double fb = (double) b / 255.0f;

    double maxVal = std::max(fr, std::max(fg, fb));
    double minVal = std::min(fr, std::min(fg, fb));

    double h, s, v;

    v = maxVal;

    if (maxVal != 0.0f) {
        s = (maxVal - minVal) / maxVal;
    } else {
        s = 0.0f;
    }

    if (s == 0.0f)
        h = 0.0f;
    else {
        double delta = maxVal - minVal;

        if (maxVal == fr) {
            h = (fg - fb) / delta;
        } else if (maxVal == fg) {
            h = 2.0f + (fb - fr) / delta;
        } else {
            h = 4.0f + (fr - fg) / delta;
        }

        h *= 60.0f;

        if (h < 0.0f) h += 360.0f;
    }

    *pH = h;
    *pS = s;
    *pV = v;
}

void HSVtoRGB(double h, double s, double v,
              Uint8 *pR, Uint8 *pG, Uint8 *pB) {
    if (s == 0.0f) {
        *pR = *pG = *pB = (Uint8) (v * 255.0f);
        return;
    }

    int r, g, b;

    h /= 60.0f;
    int i = (int) h;
    double f = h - (double) i;
    double p = v * (1.0f - s);
    double q = v * (1.0f - s * f);
    double t = v * (1.0f - s * (1.0f - f));

    switch (i) {
        case 0:
            r = (Uint8) (v * 255.0f);
            g = (Uint8) (t * 255.0f);
            b = (Uint8) (p * 255.0f);
            break;
        case 1:
            r = (Uint8) (q * 255.0f);
            g = (Uint8) (v * 255.0f);
            b = (Uint8) (p * 255.0f);
            break;
        case 2:
            r = (Uint8) (p * 255.0f);
            g = (Uint8) (v * 255.0f);
            b = (Uint8) (t * 255.0f);
            break;
        case 3:
            r = (Uint8) (p * 255.0f);
            g = (Uint8) (q * 255.0f);
            b = (Uint8) (v * 255.0f);
            break;
        case 4:
            r = (Uint8) (t * 255.0f);
            g = (Uint8) (p * 255.0f);
            b = (Uint8) (v * 255.0f);
            break;
        default:
            r = (Uint8) (v * 255.0f);
            g = (Uint8) (p * 255.0f);
            b = (Uint8) (q * 255.0f);
            break;
    }

    *pR = r;
    *pG = g;
    *pB = b;
}
