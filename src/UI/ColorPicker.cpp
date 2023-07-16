//
// Created by pyrow on 13/07/2023.
//

#include <algorithm>
#include "UI/ColorPicker.hpp"

ColorPicker::ColorPicker(double x, double y, int renderW, int renderH)
        : Button(x, y, renderW, renderH) {
    fCallOnRelease = false;
    swapColorOnClick = false;
    pickerX = (int) x;
    pickerY = (int) y;
    currentRGBA = 0;
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

static double setRGB(double *pRGB, double value) {
    *pRGB = value;
    *pRGB = std::min(*pRGB, 255.0);
    *pRGB = std::max(*pRGB, 0.0);
}

void ColorPicker::renderSelf(SDL_Renderer *gRenderer) {
    SDL_Rect dst = getRenderRect();
    double r, g, b, lastR, lastG, lastB;
    r = 255.0;
    g = 0.0;
    b = 0.0;
    const int nComb = 6;
    const double xAddRGB = ((double) nComb * 255.0) / dst.w;
    double xAddRInfo[nComb] = {0, -xAddRGB, 0, 0, xAddRGB, 0};
    double xAddGInfo[nComb] = {xAddRGB, 0, 0, -xAddRGB, 0, 0};
    double xAddBInfo[nComb] = {0, 0, xAddRGB, 0, 0, -xAddRGB};
    double yAddR, yAddG, yAddB;
    Uint8 rInt, gInt, bInt;
    int colorIndex;

    for (int i = dst.x; i < dst.x + dst.w; i++) {
        lastR = r;
        lastG = g;
        lastB = b;

        yAddR = (255.0 - r) / dst.h;
        yAddG = (255.0 - g) / dst.h;
        yAddB = (255.0 - b) / dst.h;

        for (int j = dst.y; j < dst.y + dst.h; j++) {
            rInt = (Uint8) r;
            gInt = (Uint8) g;
            bInt = (Uint8) b;
            SDL_SetRenderDrawColor(gRenderer,
                                   rInt, gInt, bInt, 255);
            SDL_RenderDrawPoint(gRenderer, i, j);

            if (i == pickerX && j == pickerY) {
                currentRGBA = (rInt << 24) | (gInt << 16) | (bInt << 8) | 0x000000FF;
            }

            setRGB(&r, r + yAddR);
            setRGB(&g, g + yAddG);
            setRGB(&b, b + yAddB);
        }

        colorIndex = ((i - dst.x) * nComb) / dst.w;

        setRGB(&r, lastR + xAddRInfo[colorIndex]);
        setRGB(&g, lastG + xAddGInfo[colorIndex]);
        setRGB(&b, lastB + xAddBInfo[colorIndex]);
    }

    int cursorSize = getPickerSize();
    SDL_Rect cursorDst = {pickerX - cursorSize / 2, pickerY - cursorSize / 2,
                          cursorSize, cursorSize};
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(gRenderer, &cursorDst);

}
