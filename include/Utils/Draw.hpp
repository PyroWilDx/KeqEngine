//
// Created by pyrow on 05/02/2024.
//

#ifndef DRAW_HPP
#define DRAW_HPP

#include <SDL2/SDL.h>

#define DEFAULT_PADDING 20

class Entity;
class Button;

class Draw {

public:
    Draw() = delete;

    static void initDraw();

    static void cleanUp();

    static Button *drawRetryButton(SDL_Rect *btRect, Entity *upperEntity,
                                void (*fOnClickRelease)(Button *, int, int, void *),
                                void *fParams);

    static Button *drawHomeButton(SDL_Rect *btRect, Entity *upperEntity, bool *gRunning);

private:

};

#endif
