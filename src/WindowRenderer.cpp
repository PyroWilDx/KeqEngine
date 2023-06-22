//
// Created by pyrowildx on 13/05/23.
//

#include <windows.h>
#include <SDL2/SDL_image.h>
#include "Utils.hpp"
#include "WindowRenderer.hpp"
#include "Entity.hpp"
#include "Particle.hpp"

WindowRenderer::WindowRenderer(const char *title, int w, int h) {
    SetProcessDPIAware();

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_SetWindowMinimumSize(window, w, h);
    myAssert(window != nullptr, "Error Creating Window.", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    myAssert(renderer != nullptr, "Error Creating Renderer.", SDL_GetError());
}

SDL_Texture *WindowRenderer::loadTexture(const char *imgPath) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, imgPath);
    myAssert(texture != nullptr, "Failed to Load Texture.", SDL_GetError());
    return texture;
}

void WindowRenderer::render(Entity *entity, Entity *background) {
    //TODO RENDERSELF WILL BE MUCH BETTER
    SDL_Rect entityFrame = entity->getFrame();

    if (entityFrame.x < 0 || entityFrame.y < 0) return;

    SDL_Rect src = entityFrame;

    float renderWMultiplier = entity->getRenderWMultiplier();
    float renderHMultiplier = entity->getRenderHMultiplier();

    float x = entity->getX();
    float y = entity->getY();

    if (background != nullptr) x -= (float) background->getFrame().x;

    int xShift = entity->getXShift();
    int yShift = entity->getYShift();

    bool facingEast = entity->isFacingEast();
    double rotation = entity->getRotation();

    if (!facingEast) {
        xShift = entity->getXShiftR();
        rotation = -rotation;
    }

    if (x > 30) {
        x += (96.0f - (float) entityFrame.w) / 2.0f;
        y += (96.0f - (float) entityFrame.h) / 2.0f;
    xShift = 0;
    yShift = 0;
    }

    SDL_Rect dst = {(int) ((float) x + (float) xShift * renderWMultiplier),
                    (int) ((float) y + (float) yShift * renderHMultiplier),
                    (int) ((float) entityFrame.w * renderWMultiplier),
                    (int) ((float) entityFrame.h * renderHMultiplier)};

    if (facingEast && rotation == 0) {
        SDL_RenderCopy(renderer, entity->getTexture(),
                       &src, &dst);
    } else {
        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (!facingEast) flip = SDL_FLIP_HORIZONTAL;
        SDL_RenderCopyEx(renderer, entity->getTexture(),
                         &src, &dst,
                         rotation, nullptr, flip);
    }

    SDL_Rect collRect = entity->getCollisionRect();
    collRect.x += (int) x;
    collRect.y += (int) y;
//    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//    SDL_RenderDrawRect(renderer, &dst);
//    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
//    SDL_RenderDrawRect(renderer, &collRect);
}

void WindowRenderer::renderParticle(Entity *particle_, Entity *background) {
    auto *particle = (Particle *) particle_;
    SDL_Rect particleFrame = particle->getFrame();

    if (particleFrame.x < 0 || particleFrame.y < 0) return;

    SDL_Rect src = particleFrame;

    Entity *entity = particle->getEntity();

    float realW, realH;
    particle->getRealSize(&realW, &realH);

    float x, y;
    int xShift, yShift, xShiftR;
    Entity *checkFaceEastEntity;
    if (particle->isEntityDependant()) {
        checkFaceEastEntity = entity;

        particle->getToEntityCenterXY(nullptr, &x, &y,
                                      &xShift, &yShift, &xShiftR);
    } else {
        checkFaceEastEntity = particle_;

        x = particle->getX();
        y = particle->getY();
        xShift = particle->getXShift();
        yShift = particle->getYShift();
        xShiftR = particle->getXShiftR();
    }

    int vXShift = xShift;
    double rotation = particle->getRotation();
    SDL_RendererFlip rendererFlip = SDL_FLIP_NONE;
    bool faceEast = checkFaceEastEntity->isFacingEast();
    if (!faceEast) {
        vXShift = xShiftR;
        rotation = -rotation;
        rendererFlip = SDL_FLIP_HORIZONTAL;
    }

    x += (float) vXShift;
    y += (float) particle->getYShift();

    if (background != nullptr) x -= (float) background->getFrame().x;

    SDL_Rect dst = {(int) x, (int) y, (int) realW, (int) realH};

    SDL_RenderCopyEx(renderer, particle->getTexture(),
                     &src, &dst,
                     rotation, nullptr,
                     rendererFlip);
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
