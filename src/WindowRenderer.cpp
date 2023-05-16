//
// Created by pyrowildx on 13/05/23.
//

#include <SDL2/SDL_image.h>
#include "Utils.hpp"
#include "WindowRenderer.hpp"

WindowRenderer::WindowRenderer(const char *title, int w, int h) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              w, h, SDL_WINDOW_SHOWN);
    myAssert(window != nullptr, "Error Creating Window.", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    myAssert(renderer != nullptr, "Error Creating Renderer.", SDL_GetError());
}

SDL_Texture *WindowRenderer::loadTexture(const char *imgPath) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, imgPath);
    myAssert(texture != nullptr, "Failed to Load Texture.", SDL_GetError());
    return texture;
}

void WindowRenderer::render(Entity *entity) {
    SDL_Rect src = entity->getFrame();
    int renderW = entity->getRenderW();
    int renderH = entity->getRenderH();
    SDL_Rect dst = {entity->getX(), entity->getY(),
                    renderW, renderH};
    if (renderW == 0) dst.w = src.w;
    if (renderH == 0) dst.h = src.h;

    SDL_RenderCopy(renderer, entity->getTexture(),
                   &src, &dst);
}

void WindowRenderer::display() {
    SDL_RenderPresent(renderer);
}

void WindowRenderer::clear() {
    SDL_RenderClear(renderer);
}

void WindowRenderer::cleanUp() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
