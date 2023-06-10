//
// Created by pyrowildx on 13/05/23.
//

#include "Entity.hpp"

Entity::Entity(int x, int y, int z) :
        frame({0, 0, 0, 0}), collisionRect(frame) {
    this->x = x;
    this->y = y;
    this->z = z;
    xVelocity = 0;
    yVelocity = 0;
    zVelocity = 0;
    facingEast = true;
    xVelocity = 0;
    hasShadow = false;
    texture = nullptr;
    renderWMultiplier = 1.0f;
    renderHMultiplier = 1.0f;
    xShift = 0;
    yShift = 0;
    xShiftR = 0;
    rotation = 0.0;
}

Entity::Entity(int x, int y, int z, int w, int h, bool hasShadow, SDL_Texture *texture)
        : Entity(x, y, z) {
    frame.x = 0;
    frame.y = 0;
    frame.w = w;
    frame.h = h;
    collisionRect = frame;
    this->hasShadow = hasShadow;
    this->texture = texture;
}

void Entity::setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetTextureColorMod(texture, r, g, b);
    SDL_SetTextureAlphaMod(texture, a);
}

void Entity::move(int dt) {
    // Z is not affected by velocity
    x += (int) ((float) dt * xVelocity);
    z += (int) ((float) dt * zVelocity);
}

void Entity::moveTo(int x_, int y_, int z_) {
    x = x_;
    y = y_;
    z = z_;
}

bool Entity::collides(Entity *entity, SDL_Rect addRect) const {
    int x1 = x + collisionRect.x + addRect.x;
    int y1 = y + collisionRect.y + addRect.y;
    int z1 = z + y1 + collisionRect.h - DEFAULT_ENTITY_LENGTH;
    int maxX1 = x1 + collisionRect.w + addRect.w;
    int maxY1 = y1 + collisionRect.h + addRect.h;
    int maxZ1 = z1 + 2 * DEFAULT_ENTITY_LENGTH;
    int x2 = entity->x + entity->collisionRect.x;
    int y2 = entity->y + entity->collisionRect.y;
    int z2 = entity->z + y2 + entity->collisionRect.h - DEFAULT_ENTITY_LENGTH;
    int maxX2 = x2 + entity->collisionRect.w;
    int maxY2 = y2 + entity->collisionRect.h;
    int maxZ2 = z2 + 2 * DEFAULT_ENTITY_LENGTH;

    if (maxX1 < x2 || x1 > maxX2) return false;
    if (maxY1 < y2 || y1 > maxY2) return false;
    if (maxZ1 < z2 || z1 > maxZ2) return false;

    return true;
}

void Entity::clearTexture() {
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void Entity::destroy() {
    clearTexture();
}
