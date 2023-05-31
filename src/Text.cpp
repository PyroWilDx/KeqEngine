//
// Created by pyrowildx on 14/05/23.
//

#include <SDL_ttf.h>
#include "Text.hpp"

Text::Text()
        : Entity(0, 0, 0) {
}

void Text::loadTextTexture(int x, int y, const char *text, const SDL_Color *color,
                           const char *fontPath, int fontSize, SDL_Renderer *renderer) {
    clearTexture();
    this->x = x;
    this->y = y;
    TTF_Font *font = TTF_OpenFont(fontPath, fontSize);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, *color);
    texture = SDL_CreateTextureFromSurface(renderer, textSurface);
    frame.w = textSurface->w;
    frame.h = textSurface->h;

    TTF_CloseFont(font);
    SDL_FreeSurface(textSurface);
}

void Text::destroy() {
    Entity::destroy();
}
