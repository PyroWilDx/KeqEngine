//
// Created by pyrowildx on 13/05/23.
//

#ifndef WINDOWRENDERER_HPP
#define WINDOWRENDERER_HPP

#include <SDL2/SDL.h>

class Entity;
class Block;

class WindowRenderer {

public:
    static void initWindowRenderer(const char *title, int w, int h);

    static inline WindowRenderer *getInstance() { return instance; }

    SDL_Texture *loadTexture(const char *imgPath);

    void render(Entity *entity);

    void render(Block *block);

    void display();

    void clear();

    void cleanUp();

    inline SDL_Window *getWindow() { return window; }

    inline SDL_Renderer *getRenderer() { return renderer; }

private:
    WindowRenderer(const char *title, int w, int h);

    static WindowRenderer *instance;

    SDL_Window *window;
    SDL_Renderer *renderer;

};

#endif
