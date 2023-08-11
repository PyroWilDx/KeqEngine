//
// Created by pyrowildx on 14/05/23.
//

#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>
#include "BaseEntity/Entity.hpp"

class Text : public Entity {

public:
    Text();

    Text(const char *text, const SDL_Color *color, const char *fontPath,
         int fontSize, bool translateBackground_ = true);

    Text(double x, double y, const char *text, const SDL_Color *color,
         const char *fontPath, int fontSize, bool translateBackground_ = true);

    ~Text() override = default;

    void loadTextTexture(const char *text, const SDL_Color *color, const char *fontPath,
                         int fontSize, bool translateBackground_ = true);

    void loadTextTextureMove(double x, double y, const char *text, const SDL_Color *color,
                             const char *fontPath, int fontSize, bool translateBackground = true);

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
