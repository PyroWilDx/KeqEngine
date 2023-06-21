//
// Created by pyrowildx on 13/05/23.
//

#include "Entity.hpp"

Entity::Entity(float x, float y) :
        frame({0, 0, 0, 0}), collisionRect(frame) {
    this->x = x;
    this->y = y;
    xVelocity = 0;
    yVelocity = 0;
    facingEast = true;
    texture = nullptr;
    renderWMultiplier = 1.0f;
    renderHMultiplier = 1.0f;
    xShift = 0;
    yShift = 0;
    xShiftR = 0;
    rotation = 0.0f;
}

Entity::Entity(float x, float y, int w, int h, SDL_Texture *texture)
        : Entity(x, y) {
    frame.x = 0;
    frame.y = 0;
    frame.w = w;
    frame.h = h;
    collisionRect = frame;
    this->texture = texture;
}

void Entity::setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetTextureColorMod(texture, r, g, b);
    SDL_SetTextureAlphaMod(texture, a);
}

void Entity::renderSelf(WindowRenderer *window) {

}

void Entity::move(int dt) {
    float addX = xVelocity * (float) dt;

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

bool Entity::collides(Entity *entity, SDL_Rect addRect) const {
    int x1 = x + collisionRect.x + addRect.x;
    int y1 = y + collisionRect.y + addRect.y;
    int maxX1 = x1 + collisionRect.w + addRect.w;
    int maxY1 = y1 + collisionRect.h + addRect.h;
    int x2 = entity->x + entity->collisionRect.x;
    int y2 = entity->y + entity->collisionRect.y;
    int maxX2 = x2 + entity->collisionRect.w;
    int maxY2 = y2 + entity->collisionRect.h;

    if (maxX1 < x2 || x1 > maxX2) return false;
    if (maxY1 < y2 || y1 > maxY2) return false;

    return true;
}

void Entity::clearTexture() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void Entity::destroy() {
    clearTexture();
}
