//
// Created by pyrowildx on 14/05/23.
//

#ifndef TEXT_HPP
#define TEXT_HPP

#include "Entity.hpp"

class Text : public Entity {

public:
    Text();

    void loadTextTexture(int x, int y, const char *text, const SDL_Color *color,
                         const char *fontPath, int fontSize, SDL_Renderer *renderer);

    void destroy() override;

private:

};

#endif
