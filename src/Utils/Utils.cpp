//
// Created by pyrowildx on 13/05/23.
//

#include <cmath>
#include <cstring>
#include <cassert>
#include <algorithm>
#include "Utils/Utils.hpp"
#include "Utils/Global.hpp"
#include "World/World.hpp"
#include "World/Background.hpp"
#include "Utils/Random.hpp"

void myAssert(bool expr, const char *msg, const char *err) {
    if (!expr) {
        SDL_Log("Assertion FAILED.");
        if (msg != nullptr) SDL_Log("Message : %s", msg);
        if (err != nullptr) SDL_Log("Error : %s", err);
        assert(expr);
    }
}

double getAddAbs(double a, double b) {
    return std::abs(a) + std::abs(b);
}

double lerp(double a, double b, double t) {
    return a + t * (b - a);
}

void fillArrayRandomInt(int intArray[], int length,
                        int start, int end) {
    for (int i = 0; i < length; i++) {
        intArray[i] = Random::getRandomInt(start, end);
    }
}

double getDistance(double x1, double y1,
                   double x2, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double atan2Pos(double y, double x) {
    double res = atan2(y, x);
    if (res < 0) res += 2 * M_PI;
    return res;
}

double getAngleOrigin(double x, double y) {
    return atan2Pos(y, x);
}

double getAngle(double x1, double y1,
                double x2, double y2,
                bool clockWise, bool invX) {
    double xDiff = (!invX) ? x2 - x1 : x1 - x2;
    double yDiff = (clockWise) ? y2 - y1 : y1 - y2;
    return getAngleOrigin(xDiff, yDiff);
}

double radToDegree(double rad) {
    return (rad * (180. / M_PI));
}

double degreeToRad(double degree) {
    return (degree * (M_PI / 180.));
}

void rotatePoint(double x, double y, double *pResX, double *pResY,
                 double xCenter, double yCenter, double angle, bool invert) {
    // Translate to Origin
    x -= xCenter;
    y -= yCenter;

    // Calc Rotation
    *pResX = x * cos(angle) - y * sin(angle);
    *pResY = x * sin(angle) + y * cos(angle);
    if (invert) *pResY = -*pResY;

    // Translate Back
    *pResX += xCenter;
    *pResY += yCenter;
}

void rotateXYArray(double xyArray[][2], const int N,
                   double xCenter, double yCenter, double angle, bool invert) {
    for (int i = 0; i < N; i++) {
        rotatePoint(xyArray[i][0], xyArray[i][1],
                    &xyArray[i][0], &xyArray[i][1],
                    xCenter, yCenter, angle, invert);
    }
}

void approxEllipse(double xyArray[][2], const int N,
                   double x, double y, double a, double b) {
    double delta = (2 * M_PI) / N;

    for (int i = 0; i < N; i++) {
        double angle = i * delta;
        double approxX = x + a * cos(angle);
        double approxY = y + b * sin(angle);
        xyArray[i][0] = approxX;
        xyArray[i][1] = approxY;
    }
}

void cutEllipseHalf(double xyArray[][2], int N, int HalfN,
                    double xyArrayHalfRight[][2], double xyArrayHalfLeft[][2]) {
    if (xyArrayHalfRight != nullptr) {
        std::memcpy(xyArrayHalfRight, xyArray,
                    sizeof(double[2]) * ((HalfN / 2) + 1));
        std::memcpy(xyArrayHalfRight + (HalfN / 2) + 1,
                    xyArray + N - (HalfN / 2), sizeof(double[2]) * (HalfN / 2));
    }

    if (xyArrayHalfLeft != nullptr) {
        std::memcpy(xyArrayHalfLeft, xyArray + (HalfN / 2),
                    sizeof(double[2]) * HalfN);
    }
}

void cutEllipseHalfHorzt(double xyArray[][2], int N, int HalfN,
                         double xyArrayHalfUp[][2], double xyArrayHalfDown[][2]) {
    if (xyArrayHalfUp != nullptr) {
        std::memcpy(xyArrayHalfUp, xyArray + HalfN - 1,
                    sizeof(double[2]) * HalfN);
        std::memcpy(xyArrayHalfUp + HalfN - 1, xyArray,
                    sizeof(double[2]));
    }

    if (xyArrayHalfDown != nullptr) {
        std::memcpy(xyArrayHalfDown, xyArray,
                    sizeof(double[2]) * HalfN);
    }
}

void shiftXYArray(double xyArray[][2], const int N,
                  double xShift, double yShift) {
    for (int i = 0; i < N; i++) {
        xyArray[i][0] += xShift;
        xyArray[i][1] += yShift;
    }
}

Uint32 cvStringToUint32(std::string &valStr) {
    unsigned long tmpL = std::stoul(valStr);
    auto res = (Uint32) tmpL;
    return res;
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
    if (pXCoeff != nullptr) {
        *pXCoeff = (double) Global::renderWindowWidth / SCREEN_BASE_WIDTH;
    }
    if (pYCoeff != nullptr) {
        *pYCoeff = (double) Global::renderWindowHeight / SCREEN_BASE_HEIGHT;
    }
}

void shiftXYFromScreenPosition(int *pX, int *pY) {
    *pX += (Global::windowWidth - Global::renderWindowWidth) / 2;
    *pY += (Global::windowHeight - Global::renderWindowHeight) / 2;
}

void getMouseAbsoluteXY(int *pMouseX, int *pMouseY) {
    int xShift = 0;
    int yShift = 0;
    shiftXYFromScreenPosition(&xShift, &yShift);
    *pMouseX -= xShift;
    *pMouseY -= yShift;
    double xCoeff, yCoeff;
    getScreenXYCoeff(&xCoeff, &yCoeff);
    *pMouseX = (int) ((double) *pMouseX / xCoeff);
    *pMouseY = (int) ((double) *pMouseY / yCoeff);
}

void getMouseAbsoluteXYNotTranslated(int *pMouseX, int *pMouseY) {
    getMouseAbsoluteXY(pMouseX, pMouseY);
    *pMouseX += Global::gWorld->getBackground()->getFrame().x;
    *pMouseY += Global::gWorld->getBackground()->getFrame().y;
}

int getSDLKeyRelation(int SDLKey, bool isKeyboard) {
    if (!isKeyboard) {
        if (SDLKey == SDL_BUTTON_LEFT) return KEY_MOUSE_LEFT;
        if (SDLKey == SDL_BUTTON_RIGHT) return KEY_MOUSE_RIGHT;

        return KEY_UNDEFINED;
    }

    if (SDLKey == SDLK_z) return KEY_Z;
    if (SDLKey == SDLK_q) return KEY_Q;
    if (SDLKey == SDLK_s) return KEY_S;
    if (SDLKey == SDLK_d) return KEY_D;
    if (SDLKey == SDLK_a) return KEY_A;
    if (SDLKey == SDLK_e) return KEY_E;
    if (SDLKey == SDLK_r) return KEY_R;
    if (SDLKey == SDLK_SPACE) return KEY_SPACE;
    if (SDLKey == SDLK_LSHIFT) return KEY_SHIFT;

    return KEY_UNDEFINED;
}

int updatePressedKeys(int SDLKey, bool keyPressed, bool isKeyboard) {
    int key = getSDLKeyRelation(SDLKey, isKeyboard);
    if (key != KEY_UNDEFINED) {
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

bool isDirectionalKeyPressed() {
    return (isKeyPressed(KEY_Q) || isKeyPressed(KEY_D));
}

void Uint32RGBAToUint8RGBA(Uint32 rgba, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a) {
    if (r != nullptr) *r = (Uint8) (rgba >> 24);
    if (g != nullptr) *g = (Uint8) (rgba >> 16);
    if (b != nullptr) *b = (Uint8) (rgba >> 8);
    if (a != nullptr) *a = (Uint8) (rgba >> 0);
}

void RGBToHSV(Uint8 r, Uint8 g, Uint8 b,
              double *pH, double *pS, double *pV) {
    double fr = (double) r / COLOR_MAX_DBL;
    double fg = (double) g / COLOR_MAX_DBL;
    double fb = (double) b / COLOR_MAX_DBL;

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
            h = 2. + (fb - fr) / delta;
        } else {
            h = 4. + (fr - fg) / delta;
        }

        h *= 60.;

        if (h < 0) h += 360.;
    }

    if (pH != nullptr) *pH = h;
    if (pS != nullptr) *pS = s;
    if (pV != nullptr) *pV = v;
}

void HSVToRGB(double h, double s, double v,
              Uint8 *pR, Uint8 *pG, Uint8 *pB) {
    if (s == 0) {
        *pR = *pG = *pB = (Uint8) (v * COLOR_MAX_DBL);
        return;
    }

    int r, g, b;

    h /= 60.;
    int i = (int) h;
    double f = h - (double) i;
    double p = v * (1. - s);
    double q = v * (1. - s * f);
    double t = v * (1. - s * (1. - f));

    switch (i) {
        case 0:
            r = (Uint8) (v * COLOR_MAX_DBL);
            g = (Uint8) (t * COLOR_MAX_DBL);
            b = (Uint8) (p * COLOR_MAX_DBL);
            break;
        case 1:
            r = (Uint8) (q * COLOR_MAX_DBL);
            g = (Uint8) (v * COLOR_MAX_DBL);
            b = (Uint8) (p * COLOR_MAX_DBL);
            break;
        case 2:
            r = (Uint8) (p * COLOR_MAX_DBL);
            g = (Uint8) (v * COLOR_MAX_DBL);
            b = (Uint8) (t * COLOR_MAX_DBL);
            break;
        case 3:
            r = (Uint8) (p * COLOR_MAX_DBL);
            g = (Uint8) (q * COLOR_MAX_DBL);
            b = (Uint8) (v * COLOR_MAX_DBL);
            break;
        case 4:
            r = (Uint8) (t * COLOR_MAX_DBL);
            g = (Uint8) (p * COLOR_MAX_DBL);
            b = (Uint8) (v * COLOR_MAX_DBL);
            break;
        default:
            r = (Uint8) (v * COLOR_MAX_DBL);
            g = (Uint8) (p * COLOR_MAX_DBL);
            b = (Uint8) (q * COLOR_MAX_DBL);
            break;
    }

    *pR = r;
    *pG = g;
    *pB = b;
}
