//
// Created by pyrowildx on 14/05/23.
//

#include <SDL2/SDL_ttf.h>
#include "Text/Text.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Colors.hpp"

Text::Text(double x, double y, const char *textStr, const SDL_Color *textColor,
           const char *fontPath, int fontSize, SDL_Color *outlColor,
           int outlSize, bool translateBackground_)
        : Entity(x, y), currText(textStr),
          currColor((textColor != nullptr) ? *textColor : Colors::dColorWhite),
          currFontPath(fontPath), currFontSize(fontSize),
          currOutlColor((outlColor != nullptr) ? *outlColor : Colors::dColorBlack),
          currOutlSize(outlSize), translateBackground(translateBackground_) {
    loadTextTexture();
}

Text::Text(double x, double y, const char *textStr, const SDL_Color *textColor,
           int fontSize, SDL_Color *outlColor, int outlSize,
           bool translateBackground_)
        : Text(x, y, textStr, textColor,
               DEFAULT_FONT_PATH, fontSize, outlColor,
               outlSize, translateBackground_) {

}

Text::Text(const char *textStr, const SDL_Color *textColor, int fontSize,
           SDL_Color *outlColor, int outlSize, bool translateBackground_)
        : Text(0, 0, textStr, textColor,
               DEFAULT_FONT_PATH, fontSize, outlColor,
               outlSize, translateBackground_) {

}

Text::Text(const char *textStr, const SDL_Color *textColor, int fontSize,
           bool translateBackground_)
        : Text(0, 0, textStr, textColor,
               DEFAULT_FONT_PATH, fontSize, nullptr,
               0, translateBackground_) {

}

Text::Text(const char *textStr, int fontSize, bool translateBackground_)
        : Text(0, 0, textStr, &Colors::dColorWhite,
               DEFAULT_FONT_PATH, fontSize, nullptr,
               0, translateBackground_) {

}

void Text::loadTextTexture() {
    clearTexture();

    SDL_Renderer *gRenderer = WindowRenderer::getInstance()->getRenderer();

    TTF_Font *textFont = TTF_OpenFont(currFontPath.c_str(), currFontSize);
    SDL_Surface *textSurface = TTF_RenderText_Solid(textFont,
                                                    currText.c_str(),
                                                    currColor);
    SDL_Surface *outSurface = nullptr;

    if (currOutlSize > 0) {
        TTF_SetFontOutline(textFont, currOutlSize);
        outSurface = TTF_RenderText_Blended(textFont,
                                            currText.c_str(),
                                            currOutlColor);
        SDL_Rect rect = {currOutlSize, currOutlSize, textSurface->w, textSurface->h};

        SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);
        SDL_BlitSurface(textSurface, nullptr, outSurface, &rect);

        imgTexture = SDL_CreateTextureFromSurface(gRenderer, outSurface);
        imgFrame.w = outSurface->w;
        imgFrame.h = outSurface->h;
    } else {
        imgTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        imgFrame.w = textSurface->w;
        imgFrame.h = textSurface->h;
    }

    TTF_CloseFont(textFont);
    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(outSurface);
}

void Text::changeText(const char *textStr) {
    currText = std::string(textStr);
    loadTextTexture();
}
