//
// Created by pyrowildx on 14/05/23.
//

#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include "AbstractEntity/Entity.hpp"

#define DEFAULT_FONT_PATH "res/fonts/MinecraftFont.ttf"

class Text : public Entity {

public:
    Text(double x, double y, const char *textStr, const SDL_Color *textColor,
         const char *fontPath, int fontSize, SDL_Color *outlColor,
         int outlSize, bool translateBackground_ = true);

    Text(double x, double y, const char *textStr, const SDL_Color *textColor,
         int fontSize, SDL_Color *outlColor, int outlSize,
         bool translateBackground_ = true);

    Text(double x, double y, const char *textStr, const SDL_Color *textColor,
         int fontSize, bool translateBackground_ = true);

    Text(double x, double y, const char *textStr, int fontSize,
         bool translateBackground_ = true);

    Text(const char *textStr, const SDL_Color *textColor, int fontSize,
         SDL_Color *outlColor, int outlSize, bool translateBackground_ = true);

    Text(const char *textStr, const SDL_Color *textColor, int fontSize,
         bool translateBackground_ = true);

    Text(const char *textStr, int fontSize, bool translateBackground_ = true);

    explicit Text(const char *textStr, bool translateBackground_ = true);

    ~Text() override = default;

    void loadTextTexture();

    void changeText(const char *textStr);

    inline bool shouldTranslate() override { return translateBackground; }

    inline void setTranslateBackground(bool translateBackground_) { translateBackground = translateBackground_; }

    [[nodiscard]] inline std::string getCurrentText() const { return currText; }

    [[nodiscard]] inline int getFontSize() const { return currFontSize; }

private:
    std::string currText;
    SDL_Color currColor;
    std::string currFontPath;
    int currFontSize;
    SDL_Color currOutlColor;
    int currOutlSize;
    bool translateBackground;

};

#endif
