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

void Entity::move(int dt, float speed) {
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
    return false;
}

void Entity::clearTexture() {
    SDL_DestroyTexture(texture);
}

void Entity::destroy() {
    clearTexture();
}
