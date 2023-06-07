//
// Created by pyrowildx on 13/05/23.
//

#include <SDL2/SDL_image.h>
#include "Utils.hpp"
#include "WindowRenderer.hpp"
#include "Keqing.hpp"
#include "Particle.hpp"

WindowRenderer::WindowRenderer(const char *title, int w, int h) {
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              w, h, SDL_WINDOW_SHOWN);
    myAssert(window != nullptr, "Error Creating Window.", SDL_GetError());

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    myAssert(renderer != nullptr, "Error Creating Renderer.", SDL_GetError());

    SDL_Texture *shadowTexture = loadTexture("res/gfx/shadow.png");
    shadow = new Entity(0, 0, 0, 32, 32, false, shadowTexture);
}

SDL_Texture *WindowRenderer::loadTexture(const char *imgPath) {
    SDL_Texture *texture = IMG_LoadTexture(renderer, imgPath);
    myAssert(texture != nullptr, "Failed to Load Texture.", SDL_GetError());
    return texture;
}

void WindowRenderer::render(Entity *entity) {
    SDL_Rect entityFrame = entity->getFrame();
    SDL_Rect src = entityFrame;
    float renderWMultiplier = entity->getRenderWMultiplier();
    float renderHMultiplier = entity->getRenderHMultiplier();
    int x = entity->getX();
    int z = entity->getZ();
    int y = entity->getY();
    int yz = y + z;
    int xShift = entity->getXShift();
    int yShift = entity->getYShift();
    SDL_Rect dst = {(int) ((float) x + (float) xShift * renderWMultiplier),
                        (int) ((float) yz + (float) yShift * renderHMultiplier),
                        (int) ((float) entityFrame.w * renderWMultiplier),
                        (int) ((float) entityFrame.h * renderHMultiplier)};

    if (entity->getHasShadow()) {
        SDL_Rect srcShadow = shadow->getFrame();
        SDL_Rect collRect = entity->getCollisionRect();
        SDL_Rect dstShadow = {x + collRect.x - collRect.w / 4,
                              DEFAULT_Y + z + collRect.y + collRect.h - collRect.h / 8,
                              collRect.w + collRect.w / 2,
                              collRect.h / 6};
        SDL_Texture *shadowTexture = shadow->getTexture();
        int color = (int) ((float) (DEFAULT_Y - y) / 2.4f);
        SDL_SetTextureColorMod(shadowTexture, color, color, color);
        SDL_RenderCopy(renderer, shadowTexture,
                       &srcShadow, &dstShadow);
    }

//    if (entity->getXDirection() >= 0) {
//        SDL_RenderCopy(renderer, entity->getTexture(),
//                       &src, &dst);
//    } else {
//        SDL_RenderCopyEx(renderer, entity->getTexture(),
//                         &src, &dst,
//                         0, nullptr, SDL_FLIP_HORIZONTAL);
//    }

//    SDL_Texture *entityTexture = entity->getTexture();
//    SDL_SetTextureColorMod(entityTexture, 255, 60, 255);
    SDL_RenderCopy(renderer, entity->getTexture(),
                   &src, &dst);

    SDL_Rect collRect = entity->getCollisionRect();
    collRect.x += x;
    collRect.y += yz;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &dst);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &collRect);
}

void WindowRenderer::renderParticle(Entity *particle_) {
    auto *particle = (Particle *) particle_;
    SDL_Rect particleFrame = particle->getFrame();
    SDL_Rect src = particleFrame;
    float renderWMultiplier = particle->getRenderWMultiplier();
    float renderHMultiplier = particle->getRenderHMultiplier();
    Entity *entity = particle->getEntity();
    int x = entity->getX();
    int z = entity->getZ();
    int y = entity->getY();
    int yz = y + z;
    int xShift = particle->getXShift();
    int yShift = particle->getYShift();
    SDL_Rect dst = {(int) ((float) x + (float) xShift * renderWMultiplier),
                    (int) ((float) yz + (float) yShift * renderHMultiplier),
                    (int) ((float) particleFrame.w * renderWMultiplier),
                    (int) ((float) particleFrame.h * renderHMultiplier)};

    SDL_RenderCopy(renderer, particle->getTexture(),
                   &src, &dst);
}

void WindowRenderer::display() {
    SDL_RenderPresent(renderer);
}

void WindowRenderer::clear() {
    SDL_RenderClear(renderer);
}

void WindowRenderer::cleanUp() {
    delete shadow;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}
