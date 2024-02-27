//
// Created by pyrowildx on 13/07/2023.
//

#include <algorithm>
#include "UI/ColorPicker.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Colors.hpp"
#include "Utils/Global.hpp"
#include "UI/VolumeSlider.hpp"
#include "World/World.hpp"

ColorPicker::ColorPicker(double x, double y, int renderW, int renderH, Uint32 currRGBA)
        : Button(x, y, renderW, renderH, 0) {
    swapColorOnClick = false;
    pickerX = WorldEntity::getX();
    pickerY = WorldEntity::getY();
//    blackSlider = new VolumeSlider(x + renderW + 10, y, renderH,
//                                   &Colors::dColorBlack, 100);
//    blackSlider->setRotation(M_PI / 2.);
//    blackSlider->setOutlineThickness(6);
//    Global::gWorld->addButton(blackSlider);

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
    *pRGB = std::min(*pRGB, (double) COLOR_MAX);
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
    r = COLOR_MAX;
    g = 0;
    b = 0;
    const int nComb = 6;
    const double xAddRGB = (double) nComb * COLOR_MAX / (double) w;
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

        yAddR = (COLOR_MAX - r) / h;
        yAddG = (COLOR_MAX - g) / h;
        yAddB = (COLOR_MAX - b) / h;

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

Uint32 ColorPicker::getCurrentRGBASlided() const {
//    Uint32 rgba = getCurrentRGBA();
//    Uint32 rgb = rgba & 0xFFFFFF00;
//    Uint32 a = rgba & 0x000000FF;
//    rgb *= (Uint32) (blackSlider->getCurrentVolume() / 100.);
//    rgb = rgb & 0xFFFFFF00;
//    return rgb + a;
    return getCurrentRGBA();
}

void ColorPicker::onClick(int mouseX, int mouseY) {
    setPickerXY(mouseX, mouseY);

    Button::onClick(mouseX, mouseY);
}

void ColorPicker::onClickedMove(int mouseX, int mouseY, bool isMouseOnButton) {
    setPickerXY(mouseX, mouseY);

    Button::onClickedMove(mouseX, mouseY, true);
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
    WindowRenderer::renderRect(&cursorDst, true,
                               0, 0, 0, COLOR_MAX,
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
