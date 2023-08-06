//
// Created by pyrowildx on 14/05/23.
//

#ifndef TEXT_HPP
#define TEXT_HPP

#include "BaseEntity/Entity.hpp"

class Text : public Entity {

public:
    Text();

    ~Text() override = default;

    void loadTextTexture(double x, double y, const char *text, const SDL_Color *color,
                         const char *fontPath, int fontSize, bool translateBackground = true);

    inline bool shouldTranslate() override { return translateBackground; }

private:
    bool translateBackground;

};

#endif
