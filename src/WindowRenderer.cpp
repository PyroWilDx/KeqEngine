//
// Created by pyrowildx on 13/05/23.
//

#ifdef _WIN32

#include <windows.h>

#endif

#include <SDL2/SDL_image.h>
#include "Utils.hpp"
#include "WindowRenderer.hpp"
#include "Entity.hpp"
#include "Global.hpp"

WindowRenderer *WindowRenderer::instance = nullptr;

WindowRenderer::WindowRenderer(const char *title, int w, int h) {

#ifdef _WIN32
    SetProcessDPIAware();
#endif

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_SetWindowMinimumSize(window, w, h);
    myAssert(window != nullptr, "Error Creating Window.", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    myAssert(renderer != nullptr, "Error Creating Renderer.", SDL_GetError());
}

void WindowRenderer::initWindowRenderer(const char *title, int w, int h) {
    if (instance == nullptr) {
        instance = new WindowRenderer(title, w, h);
    } else {
        SDL_Log("WindowRenderer already initialized!\n");
    }
}

SDL_Texture *WindowRenderer::loadTexture(const char *imgPath) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, imgPath);
    myAssert(texture != nullptr, "Failed to Load Texture.", SDL_GetError());
    return texture;
}

void WindowRenderer::render(Entity *entity) {
    SDL_Rect src = entity->getFrame();

    if (src.x < 0 || src.y < 0) return;

    SDL_Rect dst = entity->getRenderRect();

    bool facingEast = entity->isFacingEast();
    double rotation = entity->getRotation();

    if (facingEast && rotation == 0) {
        SDL_RenderCopy(renderer, entity->getTexture(),
                       &src, &dst);
    } else {
        SDL_RendererFlip renderFlip = SDL_FLIP_NONE;
        if (!facingEast) {
            rotation = -rotation;
            renderFlip = SDL_FLIP_HORIZONTAL;
        }
        SDL_RenderCopyEx(renderer, entity->getTexture(),
                         &src, &dst,
                         rotation, nullptr, renderFlip);
    }

//    SDL_Rect collRect = entity->getCollisionRect();
//    collRect.x += (int) x;
//    collRect.y += (int) y;
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    SDL_RenderDrawRect(renderer, &dst);
//    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//    SDL_RenderDrawRect(renderer, &collRect);
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
