//
// Created by pyrowildx on 13/05/23.
//

#ifdef _WIN32

#include <windows.h>

#endif

#include <SDL2/SDL_image.h>
#include "Utils/Utils.hpp"
#include "WindowRenderer.hpp"
#include "AbstractEntity/Entity.hpp"
#include "Utils/Global.hpp"
#include "World/World.hpp"
#include "World/Background.hpp"

WindowRenderer *WindowRenderer::instance = nullptr;

WindowRenderer::WindowRenderer(const char *title, int w, int h) {

#ifdef _WIN32
    SetProcessDPIAware();
#endif

    gWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_SetWindowMinimumSize(gWindow, w / 4, h / 4);
    SDL_SetWindowResizable(gWindow, SDL_TRUE);
    myAssert(gWindow != nullptr, "Error Creating Window.", SDL_GetError());;

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_PRESENTVSYNC);
    myAssert(gRenderer != nullptr, "Error Creating Renderer.", SDL_GetError());
}

WindowRenderer::~WindowRenderer() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
}

void WindowRenderer::initWindowRenderer(const char *title, int w, int h) {
    if (instance == nullptr) {
        instance = new WindowRenderer(title, w, h);
    } else {
        SDL_Log("WindowRenderer already initialized !");
    }
}

void WindowRenderer::cleanUp() {
    delete instance;
    instance = nullptr;
}

SDL_Texture *WindowRenderer::loadTexture(const char *imgPath) {
    SDL_Texture *texture = IMG_LoadTexture(gRenderer, imgPath);
    myAssert(texture != nullptr, "Failed to Load Texture.", SDL_GetError());
    return texture;
}

void WindowRenderer::renderRect(SDL_Rect *pDstRect, bool filled,
                                Uint8 r, Uint8 g, Uint8 b, Uint8 a,
                                SDL_Renderer *gRenderer_,
                                bool shifted, bool translate) {
    SDL_Rect dstRect = *pDstRect;
    if (!shifted) {
        if (translate) {
            dstRect.x -= Global::gWorld->getBackground()->getFrame().x;
            dstRect.y -= Global::gWorld->getBackground()->getFrame().y;
        }
        double xCoeff, yCoeff;
        getScreenXYCoeff(&xCoeff, &yCoeff);
        dstRect.x = (int) ((double) dstRect.x * xCoeff);
        dstRect.y = (int) ((double) dstRect.y * yCoeff);
        dstRect.w = (int) ((double) dstRect.w * xCoeff);
        dstRect.h = (int) ((double) dstRect.h * yCoeff);
        shiftXYFromScreenPosition(&dstRect.x, &dstRect.y);
    }

    SDL_SetRenderDrawBlendMode(gRenderer_, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(gRenderer_, r, g, b, a);

    if (filled) SDL_RenderFillRect(gRenderer_, &dstRect);
    else SDL_RenderDrawRect(gRenderer_, &dstRect);

    SDL_SetRenderDrawColor(gRenderer_, 0, 0, 0, 0);
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

