//
// Created by pyrowildx on 13/05/23.
//

#include "Entity.hpp"
#include "Global.hpp"

Entity::Entity(float x, float y) :
        frame({0, 0, 0, 0}), hitbox(frame) {
    this->x = x;
    this->y = y;
    xVelocity = 0;
    yVelocity = 0;
    facingEast = true;
    texture = nullptr;
    renderWMultiplier = 1.0f;
    renderHMultiplier = 1.0f;
    rotation = 0.0f;
}

Entity::Entity(float x, float y, int w, int h, SDL_Texture *texture)
        : Entity(x, y) {
    frame.x = 0;
    frame.y = 0;
    frame.w = w;
    frame.h = h;
    hitbox = frame;
    this->texture = texture;
}

Entity::~Entity() {
    clearTexture();
}

void Entity::setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetTextureColorMod(texture, r, g, b);
    SDL_SetTextureAlphaMod(texture, a);
}

void Entity::move() {
    float addX = xVelocity * (float) Global::dt;

    if (facingEast) x += addX;
    else x -= addX;
}

void Entity::moveTo(float x_, float y_) {
    x = x_;
    y = y_;
}

void Entity::moveTo(Entity *entity) {
    moveTo(entity->x, entity->y);
}

void Entity::moveTo(Entity *entity, float addX, float addY) {
    moveTo(entity);
    x += addX;
    y += addY;
}

void Entity::getRealSize(float *pW, float *pH) {
    if (pW != nullptr)
        *pW = (float) frame.w * renderWMultiplier;
    if (pH != nullptr)
        *pH = (float) frame.h * renderHMultiplier;
}

bool Entity::shouldTranslate() {
    return true;
}

SDL_Rect Entity::getRenderRect() {
    float dstX = x;
    float dstY = y;

    float realW, realH;
    getRealSize(&realW, &realH);

    if (shouldTranslate()) {
        dstX -= (float) Global::currentWorld->getBackground()->getFrame().x;
    }

    float xCoeff = (float) Global::windowWidth / SCREEN_BASE_WIDTH;
    float yCoeff = (float) Global::windowHeight / SCREEN_BASE_HEIGHT;

    SDL_Rect dst = {(int) (dstX * xCoeff), (int) (dstY * yCoeff),
                    (int) (realW * xCoeff), (int) (realH * yCoeff)};
    return dst;
}

bool Entity::collides(Entity *entity, SDL_Rect addRect) const {
    float x1 = x + hitbox.x + addRect.x;
    float y1 = y + hitbox.y + addRect.y;
    int maxX1 = x1 + hitbox.w + addRect.w;
    int maxY1 = y1 + hitbox.h + addRect.h;
    int x2 = entity->x + entity->hitbox.x;
    int y2 = entity->y + entity->hitbox.y;
    int maxX2 = x2 + entity->hitbox.w;
    int maxY2 = y2 + entity->hitbox.h;

    if (maxX1 < x2 || x1 > maxX2) return false;
    if (maxY1 < y2 || y1 > maxY2) return false;

    return true;
}

void Entity::clearTexture() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
}
