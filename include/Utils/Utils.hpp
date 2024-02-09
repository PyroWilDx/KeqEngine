//
// Created by pyrowildx on 13/05/23.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>

#define SCREEN_BASE_WIDTH 1280
#define SCREEN_BASE_HEIGHT 720

#define HUD_SB_CIRCLE_M 4.
#define HUB_SB_ICON_M (HUD_SB_CIRCLE_M * 0.74)

#define KEY_PRESS_SHORT_DURATION 100

typedef struct f2Params {
    void *fParam0;
    void *fParam1;
} f2Params;

void myAssert(bool expr, const char *msg = nullptr, const char *err = nullptr);

double getAddAbs(double a, double b);

double lerp(double a, double b, double t);

void fillArrayRandomInt(int intArray[], int length,
                        int start, int end);

double getDistance(double x1, double y1,
                   double x2, double y2);

double atan2Pos(double y, double x);

double getAngleOrigin(double x, double y);

double getAngle(double x1, double y1,
                double x2, double y2,
                bool clockWise, bool invX);

double radToDegree(double rad);

double degreeToRad(double degree);

void rotatePoint(double x, double y, double *pResX, double *pResY,
                 double xCenter, double yCenter, double angle, bool invert);

void rotateXYArray(double xyArray[][2], int N,
                   double xCenter, double yCenter, double angle, bool invert);

void approxEllipse(double xyArray[][2], int N,
                   double x, double y, double a, double b);

void cutEllipseHalf(double xyArray[][2], int N, int HalfN,
                    double xyArrayHalfRight[][2], double xyArrayHalfLeft[][2]);

void cutEllipseHalfHorzt(double xyArray[][2], int HalfN,
                    double xyArrayHalfUp[][2], double xyArrayHalfDown[][2]);

void shiftXYArray(double xyArray[][2], int N,
                  double xShift, double yShift);

template<typename T>
void clearAndShrinkVector(std::vector<T> *vector) {
    vector->clear();
    vector->shrink_to_fit();
}

template<typename T>
void removePointerElementFromVector(void *ptrEl, std::vector<T> *vector) {
    auto it = std::find(vector->begin(), vector->end(), ptrEl);
    if (it != vector->end()) {
        *it = vector->back();
        vector->pop_back();
    }
}

Uint32 cvStringToUint32(std::string &valStr);

int getTime();

void handleTime();

void getScreenXYCoeff(double *pXCoeff, double *pYCoeff);

void shiftXYFromScreenPosition(int *pX, int *pY);

void getMouseAbsoluteXY(int *pMouseX, int *pMouseY);

void getMouseAbsoluteXYNotTranslated(int *pMouseX, int *pMouseY);

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

bool isDirectionalKeyPressed();

void Uint32RGBAToUint8RGBA(Uint32 rgba, Uint8 *r, Uint8 *g, Uint8 *b, Uint8 *a);

void RGBToHSV(Uint8 r, Uint8 g, Uint8 b,
              double *pH, double *pS, double *pV);

void HSVToRGB(double h, double s, double v,
              Uint8 *pR, Uint8 *pG, Uint8 *pB);

#endif
