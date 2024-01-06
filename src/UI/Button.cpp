//
// Created by pyrowildx on 07/07/2023.
//

#include <algorithm>
#include "UI/Button.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Utils.hpp"
#include "Entity/Text.hpp"

Button::Button(double x, double y, int renderW, int renderH, bool rmOutline)
        : WorldEntity(x, y, renderW, renderH, WORLD_BUTTON),
          buttonColor() {
    this->fOnClick = nullptr;
    this->fOnClickedMove = nullptr;
    this->fOnClickRelease = nullptr;
    this->fOnDestroy = nullptr;
    this->onClickParams = nullptr;
    this->onClickedMoveParams = nullptr;
    this->onClickReleaseParams = nullptr;
    this->onDestroyParams = nullptr;
    this->swapColorOnClick = true;
    this->buttonState = BUTTON_IDLE;
    this->buttonColor = {COLOR_WHITE_FULL};
    this->buttonText = nullptr;
    this->outlineThickness = BASE_OUTLINE;
    this->outlineDarkerCoeff = 40;
    if (rmOutline) {
        translateOutline(x, y, renderW, renderH);
    }
}

Button::Button(double x, double y, int renderW, int renderH, int outlineThickness,
               bool rmOutline)
        : Button(x, y, renderW, renderH, rmOutline) {
    this->outlineThickness = outlineThickness;
    if (rmOutline) {
        translateOutline(x, y, renderW, renderH);
    }
}

Button::Button(double x, double y, int renderW, int renderH, int outlineThickness,
               int outlineDarkerCoeff,  bool rmOutline)
        : Button(x, y, renderW, renderH, outlineThickness, rmOutline) {
    this->outlineDarkerCoeff = outlineDarkerCoeff;
}

Button::~Button() {
    if (fOnDestroy != nullptr) {
        fOnDestroy(this, onDestroyParams);
    }
    delete buttonText;
}

void Button::translateOutline(double fromX, double fromY, int fromRW, int fromRH) {
    x = fromX + outlineThickness;
    y = fromY + outlineThickness;
    setRenderW(fromRW - 2 * outlineThickness);
    setRenderH(fromRH - 2 * outlineThickness);
}

void Button::changeColor(Uint8 r, Uint8 g, Uint8 b) {
    buttonColor.r = r;
    buttonColor.g = g;
    buttonColor.b = b;
}

void Button::addText(const char *text, const SDL_Color *color, int fontSize) {
    delete buttonText;

    buttonText = new Text(text, color, fontSize, false);
    buttonText->moveToEntityCenter(this);
}

void Button::changeText(const char *text) {
    buttonText->changeText(text);
    buttonText->moveToEntityCenter(this);
}

void Button::renderSelf(SDL_Renderer *gRenderer) {
    SDL_Rect dst = getRenderRect();
//    dst.x += outlineThickness;
//    dst.y += outlineThickness;
    SDL_Rect outlineRect = {dst.x - outlineThickness, dst.y - outlineThickness,
                            dst.w + outlineThickness * 2, dst.h + outlineThickness * 2};
    SDL_Color outlineRectColor = buttonColor;
    outlineRectColor.r = std::max((int) outlineRectColor.r - outlineDarkerCoeff, 0);
    outlineRectColor.g = std::max((int) outlineRectColor.g - outlineDarkerCoeff, 0);
    outlineRectColor.b = std::max((int) outlineRectColor.b - outlineDarkerCoeff, 0);
    WindowRenderer::renderRect(&outlineRect, true,
                               outlineRectColor.r, outlineRectColor.g,
                               outlineRectColor.b, outlineRectColor.a,
                               gRenderer,
                               true);

    if (imgFrame.w != 0 && imgFrame.h != 0) {
        WorldEntity::renderSelf(gRenderer);
    } else {
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
