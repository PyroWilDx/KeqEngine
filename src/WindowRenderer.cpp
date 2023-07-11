//
// Created by pyrowildx on 13/05/23.
//

#ifdef _WIN32

#include <windows.h>

#endif

#include <SDL2/SDL_image.h>
#include "Utils/Utils.hpp"
#include "WindowRenderer.hpp"
#include "EntityBase/Entity.hpp"
#include "Utils/Global.hpp"

WindowRenderer *WindowRenderer::instance = nullptr;

WindowRenderer::WindowRenderer(const char *title, int w, int h) {

#ifdef _WIN32
    SetProcessDPIAware();
#endif

    gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_SetWindowMinimumSize(gWindow, w / 4, h / 4);
    SDL_SetWindowResizable(gWindow, SDL_TRUE);
    myAssert(gWindow != nullptr, "Error Creating Window.", SDL_GetError());

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    myAssert(gRenderer != nullptr, "Error Creating Renderer.", SDL_GetError());
}

void WindowRenderer::initWindowRenderer(const char *title, int w, int h) {
    if (instance == nullptr) {
        instance = new WindowRenderer(title, w, h);
    } else {
        SDL_Log("WindowRenderer already initialized!\n");
    }
}

SDL_Texture *WindowRenderer::loadTexture(const char *imgPath) {
    SDL_Texture *texture = IMG_LoadTexture(gRenderer, imgPath);
    myAssert(texture != nullptr, "Failed to Load Texture.", SDL_GetError());
    return texture;
}

void WindowRenderer::renderEntity(Entity *entity) {
    entity->renderSelf(gRenderer);
}

void WindowRenderer::display() {
    SDL_RenderPresent(gRenderer);
}

void WindowRenderer::clear() {
    SDL_RenderClear(gRenderer);
}

void WindowRenderer::cleanUp() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
}
