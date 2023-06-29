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
#include "Block.hpp"

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
    if (src.w <= 0 || src.h <= 0) return;

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

    SDL_Rect hitbox = entity->getHitbox();
    hitbox.x += dst.x;
    hitbox.y += dst.y;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &dst);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &hitbox);
}

void WindowRenderer::render(Block *block) {
    SDL_Rect src = block->getFrame();

    if (src.x < 0 || src.y < 0) return;
    if (src.w <= 0 || src.h <= 0) return;

    SDL_Rect dst = block->getRenderRect();
    SDL_Rect backgroundFrame = Global::currentWorld->getBackground()->getFrame();
    dst.x -= backgroundFrame.x;
    dst.y -= backgroundFrame.y;

    bool facingEast = block->isFacingEast();
    double rotation = block->getRotation();

    SDL_RendererFlip renderFlip = SDL_FLIP_NONE;
    if (!facingEast) {
        rotation = -rotation;
        renderFlip = SDL_FLIP_HORIZONTAL;
    }

    int baseX = dst.x;
    int baseY = dst.y;
    double xCoeff = ((double) block->getRenderW() / (double) block->getFrame().w);
    double yCoeff = ((double) block->getRenderH() / (double) block->getFrame().h);
    for (int i = 0; i < (int) xCoeff; i++) {
        for (int j = 0; j < (int) yCoeff; j++) {
            SDL_RenderCopyEx(renderer, block->getTexture(),
                             &src, &dst,
                             rotation, nullptr, renderFlip);
            dst.y += dst.h;
        }
        dst.y = baseY;
        dst.x += dst.w;
    }

    int lastX = baseX + dst.w * (int) xCoeff;
    int lastY = baseY + dst.h * (int) yCoeff;
    int srcBaseW = src.w;
    int srcBaseH = src.h;
    int dstBaseW = dst.w;
    int dstBaseH = dst.h;
    double xLeftover = xCoeff - (int) xCoeff;
    double yLeftover = yCoeff - (int) yCoeff;
    if (xLeftover > 0.001) {
        src.w = roundToInt((double) srcBaseW * xLeftover);
        src.h = srcBaseH;
        dst.x = lastX;
        dst.y = baseY;
        dst.w = src.w;
        dst.h = dstBaseH;
        for (int j = 0; j < (int) yCoeff; j++) {
            SDL_RenderCopyEx(renderer, block->getTexture(),
                             &src, &dst,
                             rotation, nullptr, renderFlip);
            dst.y += dst.h;
        }
    }
    if (yLeftover > 0.001) {
        src.w = srcBaseW;
        src.h = roundToInt((double) srcBaseH * yLeftover);
        dst.x = baseX;
        dst.y = lastY;
        dst.w = dstBaseW;
        dst.h = src.h;
        for (int i = 0; i < (int) xCoeff; i++) {
            SDL_RenderCopyEx(renderer, block->getTexture(),
                             &src, &dst,
                             rotation, nullptr, renderFlip);
            dst.x += dst.w;
        }
    }
    if (xLeftover > 0.001 && yLeftover > 0.001) {
        src.w = roundToInt((double) srcBaseW * xLeftover);
        src.h = roundToInt((double) srcBaseH * yLeftover);
        dst.x = lastX;
        dst.y = lastY;
        dst.w = src.w;
        dst.h = src.h;
        SDL_RenderCopyEx(renderer, block->getTexture(),
                         &src, &dst,
                         rotation, nullptr, renderFlip);
    }
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
