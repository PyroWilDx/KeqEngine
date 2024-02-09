//
// Created by pyrowildx on 14/05/23.
//

#include <SDL2/SDL_ttf.h>
#include "Entity/Text.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Colors.hpp"

Text::Text(const char *textStr, int fontSize, bool translateBackground_)
        : Text(textStr, &Colors::dColorWhite, fontSize,
               translateBackground_) {

}

Text::Text(const char *textStr, const SDL_Color *textColor, int fontSize,
           bool translateBackground_)
        : Text(textStr, textColor, DEFAULT_FONT_PATH,
               fontSize, translateBackground_) {

}

Text::Text(const char *textStr, const SDL_Color *textColor, const char *fontPath,
           int fontSize, bool translateBackground_)
        : Text(0, 0, textStr, textColor,
               fontPath, fontSize, translateBackground_) {

}

Text::Text(double x, double y, const char *textStr, const SDL_Color *textColor,
           int fontSize, bool translateBackground_)
        : Text(x, y, textStr, textColor,
               DEFAULT_FONT_PATH, fontSize, translateBackground_) {

}

Text::Text(double x, double y, const char *textStr, const SDL_Color *textColor,
           const char *fontPath, int fontSize, bool translateBackground_)
        : Entity(x, y), currText(textStr),
          currColor((textColor != nullptr) ? *textColor : Colors::dColorWhite),
          currFontPath(fontPath), currFontSize(fontSize),
          translateBackground(translateBackground_) {
    loadTextTexture();
}

void Text::loadTextTexture() {
    clearTexture();

    SDL_Renderer *gRenderer = WindowRenderer::getInstance()->getRenderer();

    TTF_Font *textFont = TTF_OpenFont(currFontPath.c_str(), currFontSize);
    SDL_Surface *textSurface = TTF_RenderText_Solid(textFont,
                                                    currText.c_str(),
                                                    currColor);
    imgTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
    imgFrame.w = textSurface->w;
    imgFrame.h = textSurface->h;

    TTF_CloseFont(textFont);
    SDL_FreeSurface(textSurface);
}

void Text::changeText(const char *textStr) {
    currText = std::string(textStr);
    loadTextTexture();
}
