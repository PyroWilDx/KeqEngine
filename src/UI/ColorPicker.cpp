//
// Created by pyrow on 13/07/2023.
//

#include <algorithm>
#include "UI/ColorPicker.hpp"

ColorPicker::ColorPicker(double x, double y, int renderW, int renderH, Uint32 currRGBA)
        : Button(x, y, renderW, renderH) {
    fCallOnRelease = false;
    swapColorOnClick = false;
    pickerX = this->getX();
    pickerY = this->getY();

    absPixels = new Uint32 *[renderW];
    for (int i = 0; i < renderW; i++) {
        absPixels[i] = new Uint32[renderH];
    }
    fillPixels(currRGBA);
}

ColorPicker::~ColorPicker() {
    for (int i = 0; i < getRenderW(); i++) {
        delete[] absPixels[i];
    }
    delete[] absPixels;
}

//SDL_Rect ColorPicker::getRenderRect() {
//    SDL_Rect dst = {this->getX(), this->getY(),
//                    getRenderW(), getRenderH()};
//
//    shiftXYFromScreenPosition(&dst.x, &dst.y);
//
//    return dst;
//}

static void setRGB(double *pRGB, double value) {
    *pRGB = value;
    *pRGB = std::min(*pRGB, 255.0);
    *pRGB = std::max(*pRGB, 0.0);
}

void ColorPicker::fillPixels(Uint32 currRGBA) {
    int w = getRenderW();
    int h = getRenderH();
    int x1 = 0;
    int y1 = 0;
    int x2 = w;
    int y2 = h;

    SDL_Surface *cpSurface = SDL_CreateRGBSurface(0, w, h,
                                                  32, 0, 0, 0, 0);

    double r, g, b, lastR, lastG, lastB;
    r = 255.0;
    g = 0.0;
    b = 0.0;
    const int nComb = 6;
    const double xAddRGB = ((double) nComb * 255.0) / w;
    double xAddRInfo[nComb] = {0, -xAddRGB, 0, 0, xAddRGB, 0};
    double xAddGInfo[nComb] = {xAddRGB, 0, 0, -xAddRGB, 0, 0};
    double xAddBInfo[nComb] = {0, 0, xAddRGB, 0, 0, -xAddRGB};
    double yAddR, yAddG, yAddB;
    Uint8 rInt, gInt, bInt;
    int colorIndex;
    for (int i = x1; i < x2; i++) {
        lastR = r;
        lastG = g;
        lastB = b;

        yAddR = (255.0 - r) / h;
        yAddG = (255.0 - g) / h;
        yAddB = (255.0 - b) / h;

        for (int j = y1; j < y2; j++) {
            rInt = (Uint8) r;
            gInt = (Uint8) g;
            bInt = (Uint8) b;

            Uint32 rgba = (rInt << 24) + (gInt << 16) + (bInt << 8) + 0xFF;
            absPixels[i][j] = rgba;

            if (currRGBA != 0 && currRGBA == rgba) {
                pickerX = i;
                pickerY = j;
            }

            Uint32 *surfacePixel =
                    (Uint32 *) cpSurface->pixels + j * (cpSurface->pitch / cpSurface->format->BytesPerPixel) + i;
            *surfacePixel = SDL_MapRGB(cpSurface->format, r, g, b);

            setRGB(&r, r + yAddR);
            setRGB(&g, g + yAddG);
            setRGB(&b, b + yAddB);
        }

        colorIndex = ((i - x1) * nComb) / w;

        setRGB(&r, lastR + xAddRInfo[colorIndex]);
        setRGB(&g, lastG + xAddGInfo[colorIndex]);
        setRGB(&b, lastB + xAddBInfo[colorIndex]);
    }

    SDL_Renderer *gRenderer = WindowRenderer::getInstance()->getRenderer();
    SDL_Texture* cpTexture = SDL_CreateTextureFromSurface(gRenderer, cpSurface);
    setTexture(cpTexture);
}

Uint32 ColorPicker::getCurrentRGBA() const {
    return absPixels[pickerX][pickerY];
}

void ColorPicker::onClick(int mouseX, int mouseY) {
    pickerX = mouseX;
    pickerY = mouseY;
    Button::onClick(mouseX, mouseY);
}

int ColorPicker::getPickerSize() {
    double w, h;
    getRealSize(&w, &h);
    return ((int) (std::min(w, h) / 32.0));
}

void ColorPicker::renderSelf(SDL_Renderer *gRenderer) {
    Button::renderSelf(gRenderer);

    int cursorSize = getPickerSize();
    SDL_Rect cursorDst = {pickerX - cursorSize / 2, pickerY - cursorSize / 2,
                          cursorSize, cursorSize};
    WindowRenderer::renderRect(&cursorDst, false, false,
                               COLOR_BLACK_FULL,
                               gRenderer);
}
