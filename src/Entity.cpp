//
// Created by pyrowildx on 13/05/23.
//

#include "Entity.hpp"
#include "Global.hpp"

Entity::Entity(double x, double y) :
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

Entity::Entity(double x, double y, int w, int h, SDL_Texture *texture)
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

void Entity::moveX() {
    if (xVelocity == 0) return;

    double addX = xVelocity * (double) Global::dt;

    if (!facingEast) addX = -addX;

    x += addX;

    double yUp = y + hitbox.y;
    double yDown = yUp + hitbox.h;
    double x0, y0 = -1;
    int direction;
    if (addX > 0) {
        x0 = x + hitbox.x + hitbox.w;
        direction = KEY_Q;
    }
    if (addX < 0) {
        x0 = x + hitbox.x;
        direction = KEY_D;
    }

    if (Global::currentWorld->getPixel(x0, yUp) != BLOCK_NULL) {
        y0 = yUp;
    } else if (Global::currentWorld->getPixel(x0, yDown) != BLOCK_NULL) {
        y0 = yDown;
    }
    if (y0 != -1) {
        double xWall = Global::currentWorld->getNearestWallFrom(
                x0, y0, direction);
        x -= (x0 - xWall);
        if (addX > 0) x--;
        else x++;
    }
}

void Entity::moveY() {
    if (yVelocity == 0) return;

    y += yVelocity * (double) Global::dt;

    double xLeft = x + hitbox.x;
    double xRight = xLeft + hitbox.w;
    double x0 = -1, y0;
    int direction;
    if (yVelocity > 0) {
        y0 = y + hitbox.y + hitbox.h;
        direction = KEY_Z;
    }
    if (yVelocity < 0) {
        y0 = y + hitbox.y;
        direction = KEY_S;
    }

    if (Global::currentWorld->getPixel(xLeft, y0) != BLOCK_NULL) {
        x0 = xLeft;
    } else if (Global::currentWorld->getPixel(xRight, y0) != BLOCK_NULL) {
        x0 = xRight;
    }
    if (x0 != -1) {
        double yGround = Global::currentWorld->getNearestWallFrom(
                x0, y0, direction);
        y -= (y0 - yGround);
        if (yVelocity > 0) y--;
        else y++;
    }
}

void Entity::fallGravity() {
    double xLeft = x + hitbox.x;
    double xRight = xLeft + hitbox.w;
    double yDown = y + hitbox.y + hitbox.h;
    if (!(Global::currentWorld->getPixel(xLeft, yDown) != BLOCK_NULL ||
          Global::currentWorld->getPixel(xRight, yDown) != BLOCK_NULL)) {
        yVelocity += weight;
        moveY();
    } else {
        yVelocity = 0;
    }
}

void Entity::moveTo(double x_, double y_) {
    x = x_;
    y = y_;
}

void Entity::moveTo(Entity *entity) {
    moveTo(entity->x, entity->y);
}

void Entity::moveTo(Entity *entity, double addX, double addY) {
    moveTo(entity);
    x += addX;
    y += addY;
}

void Entity::getRealSize(double *pW, double *pH) {
    if (pW != nullptr)
        *pW = (double) frame.w * renderWMultiplier;
    if (pH != nullptr)
        *pH = (double) frame.h * renderHMultiplier;
}

bool Entity::shouldTranslate() {
    return true;
}

SDL_Rect Entity::getRenderRect() {
    double dstX = x;
    double dstY = y;

    double realW, realH;
    getRealSize(&realW, &realH);

    if (shouldTranslate()) {
        SDL_Rect backgroundFrame = Global::currentWorld->getBackground()->getFrame();
        dstX -= (double) backgroundFrame.x;
        dstY -= (double) backgroundFrame.y;
    }

    double xCoeff = (double) Global::windowWidth / SCREEN_BASE_WIDTH;
    double yCoeff = (double) Global::windowHeight / SCREEN_BASE_HEIGHT;

    SDL_Rect dst = {(int) (dstX * xCoeff), (int) (dstY * yCoeff),
                    (int) (realW * xCoeff), (int) (realH * yCoeff)};
    return dst;
}

bool Entity::collides(Entity *entity, SDL_Rect addRect) const {
    double x1 = x + hitbox.x + addRect.x;
    double y1 = y + hitbox.y + addRect.y;
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
