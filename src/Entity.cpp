//
// Created by pyrowildx on 13/05/23.
//

#include "Entity.hpp"
#include "Utils.hpp"

Entity::Entity(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
    xDirection = 0;
    yDirection = 0;
    zDirection = 0;
    speed = 0;
    frame = {0, 0, 0, 0};
    collisionRect = frame;
    hasShadow = false;
    texture = nullptr;
    renderW = 0;
    renderH = 0;
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
    renderW = w;
    renderH = h;
}

void Entity::addX(int x_) {
    x += x_;
}

void Entity::move(int dt) {
    int tmp = (int) ((float) dt * speed);
    x += xDirection * tmp;
    z += (int) ((float) zDirection * (float) tmp * Z_MULTIPLIER);
}

void Entity::moveTo(int x_, int y_, int z_) {
    x = x_;
    y = y_;
    z = z_;
}

bool Entity::collides(Entity *entity) {
    int zW = 6;
    int x1 = x + collisionRect.x;
    int y1 = y + collisionRect.y;
    int z1 = z + y1 + collisionRect.h - zW;
    int maxX1 = x1 + collisionRect.w;
    int maxY1 = y1 + collisionRect.h;
    int maxZ1 = z1 + 2 * zW;
    int x2 = entity->x + entity->collisionRect.x;
    int y2 = entity->y + entity->collisionRect.y;
    int z2 = entity->z + y2 + entity->collisionRect.h - zW;
    int maxX2 = x2 + entity->collisionRect.w;
    int maxY2 = y2 + entity->collisionRect.h;
    int maxZ2 = z2 + 2 * zW;

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
