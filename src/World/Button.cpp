//
// Created by pyrow on 07/07/2023.
//

#include "World/Button.hpp"
#include "WindowRenderer.hpp"

Button::Button(double x, double y, int renderW, int renderH)
        : WorldEntity(x, y, renderW, renderH, WORLD_BUTTON), buttonColor() {
    fCallBack = nullptr;
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
    if (imgFrame.w != 0 && imgFrame.h != 0) {
        Entity::renderSelf(gRenderer);
    } else {
        SDL_SetRenderDrawColor(gRenderer, buttonColor.r, buttonColor.g, buttonColor.b, buttonColor.a);
        SDL_Rect dst = getRenderRect();
        SDL_RenderFillRect(gRenderer, &dst);
    }
    if (buttonText != nullptr) buttonText->renderSelf(gRenderer);
}

void Button::onClick() {
    fCallBack(nullptr); // TODO
}
