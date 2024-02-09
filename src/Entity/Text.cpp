//
// Created by pyrowildx on 14/05/23.
//

#include <SDL2/SDL_ttf.h>
#include "Entity/Text.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Colors.hpp"

Text::Text(const char *textStr, int fontSize, bool translateBackground_) {
    loadTextTexture(textStr, &Colors::dColorWhite, DEFAULT_FONT_PATH,
                    fontSize, translateBackground_);
}

Text::Text(const char *textStr, const SDL_Color *textColor,
           int fontSize, bool translateBackground_) {
    loadTextTexture(textStr, textColor, DEFAULT_FONT_PATH,
                    fontSize, translateBackground_);
}

Text::Text(const char *textStr, const SDL_Color *textColor, const char *fontPath,
           int fontSize, bool translateBackground_) {
    loadTextTexture(textStr, textColor, fontPath,
                    fontSize, translateBackground_);
}

Text::Text(double x, double y, const char *textStr, const SDL_Color *textColor,
           int fontSize, bool translateBackground_) {
    loadTextTexture(textStr, textColor, DEFAULT_FONT_PATH,
                    fontSize, translateBackground_);
    this->x = x;
    this->y = y;
}

Text::Text(double x, double y, const char *textStr, const SDL_Color *textColor,
           const char *fontPath, int fontSize, bool translateBackground_) {
    loadTextTexture(textStr, textColor, fontPath,
                    fontSize, translateBackground_);
    this->x = x;
    this->y = y;
}

void Text::loadTextTexture(const char *textStr, const SDL_Color *textColor, const char *fontPath,
                           int fontSize, bool translateBackground_) {
    clearTexture();

    if (textColor != nullptr) currColor = *textColor;
    else currColor = Colors::dColorWhite;

    TTF_Font *font = TTF_OpenFont(fontPath, fontSize);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, textStr, currColor);
    imgTexture = SDL_CreateTextureFromSurface(WindowRenderer::getInstance()->getRenderer(), textSurface);
    imgFrame.w = textSurface->w;
    imgFrame.h = textSurface->h;

    TTF_CloseFont(font);
    SDL_FreeSurface(textSurface);

    currText = std::string(textStr);
    currFontPath = std::string(fontPath);
    currFontSize = fontSize;
    translateBackground = translateBackground_;
}

void Text::changeText(const char *textStr) {
    loadTextTexture(textStr, &currColor, currFontPath.c_str(),
                    currFontSize, translateBackground);
}
