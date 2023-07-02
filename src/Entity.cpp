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
    gravityWeight = 0;
    facingEast = true;
    texture = nullptr;
    renderWMultiplier = 1;
    renderHMultiplier = 1;
    rotation = 0;
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

    double yUp = y + hitbox.y + 1;
    double yDown = y + hitbox.y + hitbox.h - 1;

    if (addX > 0) {
        double xRight = x + hitbox.x + hitbox.w;
        double xWall = Global::currentWorld->getNearestWallFrom(
                xRight, yUp, KEY_Q);
        xWall = min(xWall, Global::currentWorld->getNearestWallFrom(
                xRight, yDown, KEY_Q));
        if (xRight > xWall) {
            x -= (xRight - xWall);
        }
    }

    if (addX < 0) {
        double xLeft = x + hitbox.x;
        double xWall = Global::currentWorld->getNearestWallFrom(
                xLeft, yUp, KEY_D);
        xWall = max(xWall, Global::currentWorld->getNearestWallFrom(
                xLeft, yDown, KEY_D));
        if (xLeft < xWall) {
            x -= (xLeft - xWall);
        }
    }
}

void Entity::moveY() {
    if (yVelocity == 0) return;

    y += yVelocity * (double) Global::dt;

    double xLeft = x + hitbox.x + 1;
    double xRight = x + hitbox.x + hitbox.w - 1;

    if (yVelocity > 0) {
        double yDown = y + hitbox.y + hitbox.h;
        double yWall = Global::currentWorld->getNearestWallFrom(
                xLeft, yDown, KEY_Z);
        yWall = min(yWall, Global::currentWorld->getNearestWallFrom(
                xRight, yDown, KEY_Z));
        if (yDown > yWall) {
            y -= (yDown - yWall);
        }
    }

    if (yVelocity < 0) {
        double yUp = y + hitbox.y;
        double yWall = Global::currentWorld->getNearestWallFrom(
                xLeft, yUp, KEY_S);
        yWall = max(yWall, Global::currentWorld->getNearestWallFrom(
                xRight, yUp, KEY_S));
        if (yUp < yWall) {
            y -= (yUp - yWall);
        }
    }
}

void Entity::fallGravity() {
    double xLeft = x + hitbox.x + 1;
    double xRight = x + hitbox.x + hitbox.w - 1;
    double yDown = y + hitbox.y + hitbox.h;
    if (!(Global::currentWorld->getPixel(xLeft, yDown) != BLOCK_NULL ||
          Global::currentWorld->getPixel(xRight, yDown) != BLOCK_NULL)) {
        yVelocity += gravityWeight * (double) Global::dt;
    } else {
        if (yVelocity > 0) {
            yVelocity = 0;
        }
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

    SDL_Rect dst = {roundToInt(dstX * xCoeff), roundToInt(dstY * yCoeff),
                    roundToInt(realW * xCoeff), roundToInt(realH * yCoeff)};
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
