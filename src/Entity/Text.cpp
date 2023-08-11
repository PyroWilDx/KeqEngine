//
// Created by pyrowildx on 14/05/23.
//

#include <SDL2/SDL_ttf.h>
#include "Entity/Text.hpp"
#include "Utils/Utils.hpp"
#include "WindowRenderer.hpp"

Text::Text()
        : Entity() {
    this->currText = "";
    this->currColor = {COLOR_WHITE_FULL};
    this->currFontPath = "";
    this->currFontSize = 0;
    this->translateBackground = true;
}

Text::Text(const char *text, const SDL_Color *color, const char *fontPath,
           int fontSize, bool translateBackground_) {
    loadTextTexture(text, color, fontPath,
                    fontSize, translateBackground_);
}

Text::Text(double x, double y, const char *text, const SDL_Color *color,
           const char *fontPath, int fontSize, bool translateBackground_)
        : Text() {
    loadTextTextureMove(x, y, text, color,
                        fontPath, fontSize, translateBackground_);
}

void Text::loadTextTexture(const char *text, const SDL_Color *color, const char *fontPath,
                           int fontSize, bool translateBackground_) {
    TTF_Font *font = TTF_OpenFont(fontPath, fontSize);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, *color);
    imgTexture = SDL_CreateTextureFromSurface(WindowRenderer::getInstance()->getRenderer(), textSurface);
    imgFrame.w = textSurface->w;
    imgFrame.h = textSurface->h;

    TTF_CloseFont(font);
    SDL_FreeSurface(textSurface);

    this->currText = std::string(text);
    this->currColor = *color;
    this->currFontPath = std::string(fontPath);
    this->currFontSize = fontSize;
    this->translateBackground = translateBackground_;
}

void Text::loadTextTextureMove(double x, double y, const char *text, const SDL_Color *color,
                               const char *fontPath, int fontSize, bool translateBackground_) {
    clearTexture();

    loadTextTexture(text, color, fontPath, fontSize, translateBackground_);

    this->x = x;
    this->y = y;
}

void Text::changeText(const char *text) {
    loadTextTexture(text, &currColor, currFontPath.c_str(),
                    currFontSize, translateBackground);
}
