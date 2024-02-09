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
    Text(const char *textStr, int fontSize, bool translateBackground_ = true);

    Text(const char *textStr, const SDL_Color *textColor,
         int fontSize, bool translateBackground_ = true);

    Text(const char *textStr, const SDL_Color *textColor, const char *fontPath,
         int fontSize, bool translateBackground_ = true);

    Text(double x, double y, const char *textStr, const SDL_Color *textColor,
         int fontSize, bool translateBackground_ = true);

    Text(double x, double y, const char *textStr, const SDL_Color *textColor,
         const char *fontPath, int fontSize, bool translateBackground_ = true);

    ~Text() override = default;

    void loadTextTexture(const char *textStr, const SDL_Color *textColor, const char *fontPath,
                         int fontSize, bool translateBackground_ = true);

    void changeText(const char *textStr);

    inline bool shouldTranslate() override { return translateBackground; }

    [[nodiscard]] inline std::string getCurrentText() const { return currText; }

private:
    std::string currText;
    SDL_Color currColor;
    std::string currFontPath;
    int currFontSize;
    bool translateBackground;

};

#endif
