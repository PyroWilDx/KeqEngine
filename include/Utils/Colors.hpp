//
// Created by pyrow on 09/02/2024.
//

#ifndef COLORS_HPP
#define COLORS_HPP

#include <SDL2/SDL.h>

#define COLOR_MAX_C 0xFF

class Colors {

public:
    Colors() = delete;

    static void initColors();

    static void cleanUp();

    static SDL_Color dColorKq;

private:

};

#endif
