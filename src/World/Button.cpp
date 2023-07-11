//
// Created by pyrowildx on 07/07/2023.
//

#include <algorithm>
#include "World/Button.hpp"
#include "WindowRenderer.hpp"

Button::Button(double x, double y, int renderW, int renderH)
        : WorldEntity(x, y, renderW, renderH, WORLD_BUTTON),
          buttonColor() {
    fCallBack = nullptr;
    onClickParams = nullptr;
    buttonState = BUTTON_IDLE;
    buttonColor = {RGBA_FULL};
    buttonText = nullptr;
}

void Button::changeColor(Uint8 r, Uint8 g, Uint8 b) {
    buttonColor.r = r;
    buttonColor.g = g;
    buttonColor.b = b;
}

void Button::addText(const char *text, const SDL_Color *color,
                     const char *fontPath, int fontSize) {
    buttonText = new Text();
    buttonText->loadTextTexture(x, y, text, color,
                                fontPath, fontSize, false);
    buttonText->moveToEntityCenter(this);
}

void Button::renderSelf(SDL_Renderer *gRenderer) {
    SDL_Rect dst = getRenderRect();
    int outlineThickness = 6;
    SDL_Rect outlineRect = {dst.x - outlineThickness, dst.y - outlineThickness,
                            dst.w + outlineThickness * 2, dst.h + outlineThickness * 2};
    SDL_Color outlineRectColor = buttonColor;
    int darkerCoeff = 40;
    outlineRectColor.r = std::max(outlineRectColor.r - darkerCoeff, 0);
    outlineRectColor.g = std::max(outlineRectColor.g - darkerCoeff, 0);
    outlineRectColor.b = std::max(outlineRectColor.b - darkerCoeff, 0);
    SDL_SetRenderDrawColor(gRenderer, outlineRectColor.r, outlineRectColor.g,
                           outlineRectColor.b, outlineRectColor.a);
    SDL_RenderFillRect(gRenderer, &outlineRect);

    if (imgFrame.w != 0 && imgFrame.h != 0) {
        Entity::renderSelf(gRenderer);
    } else {
        SDL_Color renderColor = buttonColor;
        if (buttonState == BUTTON_CLICKED) {
            renderColor.r = renderColor.r / 2;
            renderColor.g = renderColor.g / 2;
            renderColor.b = renderColor.b / 2;
        }
        SDL_SetRenderDrawColor(gRenderer, renderColor.r, renderColor.g,
                               renderColor.b, renderColor.a);
        SDL_RenderFillRect(gRenderer, &dst);
    }
    if (buttonText != nullptr) buttonText->renderSelf(gRenderer);
}

void Button::onClick() {
    buttonState = BUTTON_CLICKED;
}

void Button::onClickRelease(bool isMouseOnButton) {
    if (isMouseOnButton) fCallBack(onClickParams);
    buttonState = BUTTON_IDLE;
}
