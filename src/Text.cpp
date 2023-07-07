//
// Created by pyrowildx on 14/05/23.
//

#include <SDL2/SDL_ttf.h>
#include "Text.hpp"
#include "WindowRenderer.hpp"

Text::Text()
        : Entity(0, 0) {
    translateBackground = true;
}

void Text::loadTextTexture(double x, double y, const char *text, const SDL_Color *color,
                           const char *fontPath, int fontSize, bool translateBackground_) {
    clearTexture();
    this->x = x;
    this->y = y;
    TTF_Font *font = TTF_OpenFont(fontPath, fontSize);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, *color);
    imgTexture = SDL_CreateTextureFromSurface(WindowRenderer::getInstance()->getRenderer(), textSurface);
    imgFrame.w = textSurface->w;
    imgFrame.h = textSurface->h;
    this->translateBackground = translateBackground_;

    TTF_CloseFont(font);
    SDL_FreeSurface(textSurface);
}
