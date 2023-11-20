//
// Created by pyrowildx on 13/07/2023.
//

#include <algorithm>
#include "UI/ColorPicker.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Utils.hpp"

ColorPicker::ColorPicker(double x, double y, int renderW, int renderH, Uint32 currRGBA)
        : Button(x + 8, y + 8, renderW, renderH,
                 8, 20) {
    swapColorOnClick = false;
    pickerX = WorldEntity::getX();
    pickerY = WorldEntity::getY();

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

static void setRGB(double *pRGB, double value) {
    *pRGB = value;
    *pRGB = std::min(*pRGB, COLOR_MAX_DBL);
    *pRGB = std::max(*pRGB, 0.);
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
    r = COLOR_MAX_DBL;
    g = 0;
    b = 0;
    const int nComb = 6;
    const double xAddRGB = (double) nComb * COLOR_MAX_DBL / (double) w;
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

        yAddR = (COLOR_MAX_DBL - r) / h;
        yAddG = (COLOR_MAX_DBL - g) / h;
        yAddB = (COLOR_MAX_DBL - b) / h;

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
            *surfacePixel = SDL_MapRGB(cpSurface->format, rInt, gInt, bInt);

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
    SDL_Texture *cpTexture = SDL_CreateTextureFromSurface(gRenderer, cpSurface);
    setTexture(cpTexture);
    SDL_FreeSurface(cpSurface);
}

Uint32 ColorPicker::getCurrentRGBA() const {
    return absPixels[pickerX][pickerY];
}

void ColorPicker::onClick(int mouseX, int mouseY) {
    setPickerXY(mouseX, mouseY);

    Button::onClick(mouseX, mouseY);
}

void ColorPicker::onClickedMove(int mouseX, int mouseY, bool isMouseOnButton) {
    setPickerXY(mouseX, mouseY);

    Button::onClickedMove(mouseX, mouseY, isMouseOnButton);
}

int ColorPicker::getPickerSize() {
    double w, h;
    getRealSize(&w, &h);
    return (int) (std::min(w, h) / 32.);
}

void ColorPicker::renderSelf(SDL_Renderer *gRenderer) {
    Button::renderSelf(gRenderer);

    int cursorSize = getPickerSize();
    SDL_Rect cursorDst = {pickerX - cursorSize / 2, pickerY - cursorSize / 2,
                          cursorSize, cursorSize};
    cursorDst.x += (int) x;
    cursorDst.y += (int) y;
    WindowRenderer::renderRect(&cursorDst, false,
                               COLOR_BLACK_FULL,
                               gRenderer,
                               false, false);
}

void ColorPicker::setPickerXY(int mouseX, int mouseY) {
    int renderW = getRenderW();
    int renderH = getRenderH();
    pickerX = (int) ((double) mouseX - x);
    if (pickerX < 0) pickerX = 0;
    if (pickerX >= renderW) pickerX = renderW - 1;
    pickerY = (int) ((double) mouseY - y);
    if (pickerY < 0) pickerY = 0;
    if (pickerY >= renderH) pickerY = renderH - 1;
}
