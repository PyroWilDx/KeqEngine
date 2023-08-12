//
// Created by pyrowildx on 07/07/2023.
//

#include <algorithm>
#include "UI/Button.hpp"
#include "WindowRenderer.hpp"

Button::Button(double x, double y, int renderW, int renderH)
        : WorldEntity(x, y, renderW, renderH, WORLD_BUTTON),
          buttonColor() {
    fOnClick = nullptr;
    fOnClickedMove = nullptr;
    fOnClickRelease = nullptr;
    onClickParams = nullptr;
    onClickedMoveParams = nullptr;
    onClickReleaseParams = nullptr;
    swapColorOnClick = true;
    buttonState = BUTTON_IDLE;
    buttonColor = {COLOR_WHITE_FULL};
    buttonText = nullptr;
    outlineThickness = 6;
}

Button::Button(double x, double y, int renderW, int renderH, int outlineThickness)
        : Button(x, y, renderW, renderH) {
    this->outlineThickness = outlineThickness;
}


Button::~Button() {
    delete buttonText;
}

void Button::changeColor(Uint8 r, Uint8 g, Uint8 b) {
    buttonColor.r = r;
    buttonColor.g = g;
    buttonColor.b = b;
}

void Button::addText(const char *text, const SDL_Color *color,
                     const char *fontPath, int fontSize) {
    delete buttonText;

    buttonText = new Text(text, color, fontPath,
                          fontSize, false);
    buttonText->moveToEntityCenter(this);
}

void Button::changeText(const char *text) {
    buttonText->changeText(text);
    buttonText->moveToEntityCenter(this);
}

void Button::renderSelf(SDL_Renderer *gRenderer) {
    SDL_Rect dst = getRenderRect();

    if (imgFrame.w != 0 && imgFrame.h != 0) {
        Entity::renderSelf(gRenderer);
    } else {
        SDL_Rect outlineRect = {dst.x - outlineThickness, dst.y - outlineThickness,
                                dst.w + outlineThickness * 2, dst.h + outlineThickness * 2};
        SDL_Color outlineRectColor = buttonColor;
        int darkerCoeff = 40;
        outlineRectColor.r = std::max(outlineRectColor.r - darkerCoeff, 0);
        outlineRectColor.g = std::max(outlineRectColor.g - darkerCoeff, 0);
        outlineRectColor.b = std::max(outlineRectColor.b - darkerCoeff, 0);
        WindowRenderer::renderRect(&outlineRect, true,
                                   outlineRectColor.r, outlineRectColor.g,
                                   outlineRectColor.b, outlineRectColor.a,
                                   gRenderer,
                                   true);

        SDL_Color renderColor = buttonColor;
        if (buttonState == BUTTON_CLICKED) {
            if (swapColorOnClick) {
                renderColor.r = renderColor.r / 2;
                renderColor.g = renderColor.g / 2;
                renderColor.b = renderColor.b / 2;
            }
        }
        WindowRenderer::renderRect(&dst, true,
                                   renderColor.r, renderColor.g,
                                   renderColor.b, renderColor.a,
                                   gRenderer,
                                   true);
    }
    if (buttonText != nullptr) buttonText->renderSelf(gRenderer);
}

void Button::onClick(int mouseX, int mouseY) {
    buttonState = BUTTON_CLICKED;
    if (fOnClick != nullptr) fOnClick(this, mouseX, mouseY, onClickParams);
}

void Button::onClickedMove(int mouseX, int mouseY, bool isMouseOnButton) {
    if (isMouseOnButton && fOnClickedMove != nullptr) {
        fOnClickedMove(this, mouseX, mouseY, onClickedMoveParams);
    }
}

void Button::onClickRelease(int mouseX, int mouseY, bool isMouseOnButton) {
    buttonState = BUTTON_IDLE;
    if (isMouseOnButton && fOnClickRelease != nullptr) {
        fOnClickRelease(this, mouseX, mouseY, onClickReleaseParams);
    }
}
