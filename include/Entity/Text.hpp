//
// Created by pyrowildx on 14/05/23.
//

#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include "BaseEntity/Entity.hpp"

#define DEFAULT_FONT_PATH "res/fonts/JetBrainsMono-Regular.ttf"

class Text : public Entity {

public:
    Text(const char *text, int fontSize, bool translateBackground_ = true);

    Text(const char *text, const SDL_Color *color,
               int fontSize, bool translateBackground_ = true);

    Text(const char *text, const SDL_Color *color, const char *fontPath,
         int fontSize, bool translateBackground_ = true);

    Text(double x, double y, const char *text, const SDL_Color *color,
         int fontSize, bool translateBackground_ = true);

    Text(double x, double y, const char *text, const SDL_Color *color,
         const char *fontPath, int fontSize, bool translateBackground_ = true);

    ~Text() override = default;

    void loadTextTexture(const char *text, const SDL_Color *color, const char *fontPath,
                         int fontSize, bool translateBackground_ = true);

    void changeText(const char *text);

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
