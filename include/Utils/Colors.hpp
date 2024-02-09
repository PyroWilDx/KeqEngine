//
// Created by pyrow on 09/02/2024.
//

#ifndef COLORS_HPP
#define COLORS_HPP

#include <SDL2/SDL.h>

#define COLOR_MAX 0xFF

class Colors {

public:
    Colors() = delete;

    static void initColors();

    static void cleanUp();

    static SDL_Color dColorKq;
    static SDL_Color dColorElectroDmgText;
    static SDL_Color dColorWhite;
    static SDL_Color dColorBlack;
    static SDL_Color dColorGray;
    static SDL_Color dColorRed;
    static SDL_Color dColorGreen;
    static SDL_Color dColorBlue;
    static SDL_Color dColorCyan;

private:

};

#endif
