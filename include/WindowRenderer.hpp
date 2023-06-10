//
// Created by pyrowildx on 13/05/23.
//

#ifndef WINDOWRENDERER_HPP
#define WINDOWRENDERER_HPP

#include <SDL2/SDL.h>
#include "Entity.hpp"

class WindowRenderer {

public:
    WindowRenderer(const char *title, int w, int h);

    SDL_Texture *loadTexture(const char *imgPath);

    void render(Entity *entity, Entity *background);

    void renderParticle(Entity *particle, Entity *background);

    void display();

    void clear();

    void cleanUp();

    inline SDL_Renderer *getRenderer() { return renderer; }

private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    Entity *shadow;

};

#endif
