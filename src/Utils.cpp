//
// Created by pyrowildx on 13/05/23.
//

#include <cassert>
#include <cstdio>
#include <algorithm>
#include "Utils.hpp"

void myAssert(bool expr, const char *msg, const char *err) {
    if (!expr) {
        printf("Message : %s\n", msg);
        printf("Error : %s\n", err);
        assert(expr);
    }
}

bool isNewestFrame(Sprite *sprite, int x) {
    return (sprite->frameX == x && sprite->accumulatedTime == 0);
}

bool isSameSpriteCode(Sprite *sprite, int spriteCode) {
    return (sprite->code == spriteCode);
}

void RGBtoHSV(Uint8 r, Uint8 g, Uint8 b,
              float *pH, float *pS, float *pV) {
    float fr = (float) r / 255.0f;
    float fg = (float) g / 255.0f;
    float fb = (float) b / 255.0f;

    float maxVal = std::max(fr, std::max(fg, fb));
    float minVal = std::min(fr, std::min(fg, fb));

    float h, s, v;

    v = maxVal;

    if (maxVal != 0.0f) {
        s = (maxVal - minVal) / maxVal;
    } else {
        s = 0.0f;
    }

    if (s == 0.0f)
        h = 0.0f;
    else {
        float delta = maxVal - minVal;

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

void HSVtoRGB(float h, float s, float v,
              Uint8 *pR, Uint8 *pG, Uint8 *pB) {
    if (s == 0.0f) {
        *pR = *pG = *pB = (Uint8) (v * 255.0f);
        return;
    }

    int r, g, b;

    h /= 60.0f;
    int i = (int) h;
    float f = h - (float) i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

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
