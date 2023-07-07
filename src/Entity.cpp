//
// Created by pyrowildx on 13/05/23.
//

#include "Entity.hpp"
#include "Global.hpp"

Entity::Entity(double x, double y) :
        imgFrame({0, 0, 0, 0}), hitBox(imgFrame) {
    this->x = x;
    this->y = y;
    xVelocity = 0;
    yVelocity = 0;
    gravityWeight = 0;
    facingEast = true;
    imgTexture = nullptr;
    renderWMultiplier = 1;
    renderHMultiplier = 1;
    degRotation = 0;
}

Entity::Entity(double x, double y, int w, int h, SDL_Texture *texture)
        : Entity(x, y) {
    imgFrame.x = 0;
    imgFrame.y = 0;
    imgFrame.w = w;
    imgFrame.h = h;
    hitBox = imgFrame;
    this->imgTexture = texture;
}

Entity::~Entity() {
    clearTexture();
}

void Entity::setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetTextureColorMod(imgTexture, r, g, b);
    SDL_SetTextureAlphaMod(imgTexture, a);
}

void Entity::checkXCollision(bool checkRight) {
    double yHitbox = y + hitBox.y;
    const double yToCheck[5] = {
            yHitbox + 1, // 0%
            yHitbox + (double) hitBox.h / 4.0, // 25%
            yHitbox + (double) hitBox.h / 2.0, // 50%
            yHitbox + (double) hitBox.h / (4.0 / 3.0), // 75%
            yHitbox + (double) hitBox.h - 1 // 100%
    };

    if (checkRight) {
        double xRight = x + hitBox.x + hitBox.w;
        double xWall = INT32_MAX;
        for (double vY: yToCheck) {
            xWall = min(xWall, Global::currentWorld->getNearestWallFrom(
                    xRight, vY, KEY_Q));
        }
        if (xRight > xWall) {
            x = xWall - hitBox.w - hitBox.x;
        }

    } else {
        double xLeft = x + hitBox.x;
        double xWall = -INT32_MAX;
        for (double vY: yToCheck) {
            xWall = max(xWall, Global::currentWorld->getNearestWallFrom(
                    xLeft, vY, KEY_D));
        }
        if (xLeft < xWall) {
            x = xWall - hitBox.x;
        }
    }
}

void Entity::moveX() {
    double addX = xVelocity * (double) Global::dt;

    if (addX == 0) return;

    if (!facingEast) addX = -addX;
    x += addX;

    checkXCollision((addX > 0));
}

void Entity::checkYCollision(bool checkDown) {
    double xLeft = x + hitBox.x + 1;
    double xRight = x + hitBox.x + hitBox.w - 1;

    if (checkDown) {
        double yDown = y + hitBox.y + hitBox.h;
        double yWall = Global::currentWorld->getNearestWallFrom(
                xLeft, yDown, KEY_Z);
        yWall = min(yWall, Global::currentWorld->getNearestWallFrom(
                xRight, yDown, KEY_Z));
        if (yDown > yWall) {
            y = yWall - hitBox.h - hitBox.y;
        }

    } else {
        double yUp = y + hitBox.y;
        double yWall = Global::currentWorld->getNearestWallFrom(
                xLeft, yUp, KEY_S);
        yWall = max(yWall, Global::currentWorld->getNearestWallFrom(
                xRight, yUp, KEY_S));
        if (yUp < yWall) {
            y = yWall - hitBox.y;
            yVelocity = 0;
        }
    }
}

void Entity::moveY() {
    if (yVelocity == 0) return;

    y += yVelocity * (double) Global::dt;

    checkYCollision((yVelocity > 0));
}

bool Entity::isInAir() const {
    double xLeft = x + hitBox.x + 1;
    double xRight = x + hitBox.x + hitBox.w - 1;
    double yDown = y + hitBox.y + hitBox.h;
    return (Global::currentWorld->getPixel(xLeft, yDown) == BLOCK_NULL &&
            Global::currentWorld->getPixel(xRight, yDown) == BLOCK_NULL);
}

void Entity::fallGravity() {
    if (isInAir()) {
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
        *pW = (double) imgFrame.w * renderWMultiplier;
    if (pH != nullptr)
        *pH = (double) imgFrame.h * renderHMultiplier;
}

bool Entity::shouldTranslate() {
    return true;
}

SDL_Rect Entity::getRenderRect() {
    double dstX = getX();
    double dstY = getY();

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

void Entity::addRenderWHMultiplier(double addW, double addH, double maxW, double maxH) {
    if (addW != 0) {
        renderWMultiplier += addW;
        if (maxW > 0 && renderWMultiplier > maxW) renderWMultiplier = maxW;
    }
    if (addH != 0) {
        renderHMultiplier += addH;
        if (maxH > 0 && renderHMultiplier > maxH) renderHMultiplier = maxH;
    }
}

void Entity::addRenderWHMultiplierR(double addW, double addH, double maxW, double maxH) {
    double lastRenderWM = renderWMultiplier;
    double lastRenderHM = renderHMultiplier;
    double lastW, lastH;
    getRealSize(&lastW, &lastH);

    addRenderWHMultiplier(addW, addH, maxW, maxH);

    double newW, newH;
    getRealSize(&newW, &newH);

    if (lastRenderWM != renderWMultiplier) x -= (newW - lastW);
    if (lastRenderHM != renderHMultiplier) y -= (newH - lastH);
}

void Entity::renderSelf(SDL_Renderer *gRenderer) {
    SDL_Rect src = imgFrame;

    if (src.x < 0 || src.y < 0) return;
    if (src.w <= 0 || src.h <= 0) return;

    SDL_Rect dst = this->getRenderRect();

    if (facingEast && degRotation == 0) {
        SDL_RenderCopy(gRenderer, imgTexture,
                       &src, &dst);
    } else {
        SDL_RendererFlip renderFlip = SDL_FLIP_NONE;
        double renderRotation = degRotation;
        if (!facingEast) {
            renderFlip = SDL_FLIP_HORIZONTAL;
            renderRotation = -degRotation;
        }
        SDL_RenderCopyEx(gRenderer, imgTexture,
                         &src, &dst,
                         renderRotation, nullptr, renderFlip);
    }

    SDL_Rect dstHitBox = hitBox;
    dstHitBox.x += getX() - Global::currentWorld->getBackground()->getFrame().x;
    dstHitBox.y += getY() - Global::currentWorld->getBackground()->getFrame().y;
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(gRenderer, &dst);
    SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(gRenderer, &dstHitBox);
}

bool Entity::collides(Entity *entity, SDL_Rect addRect) const {
    double x1 = x + hitBox.x + addRect.x;
    double y1 = y + hitBox.y + addRect.y;
    int maxX1 = x1 + hitBox.w + addRect.w;
    int maxY1 = y1 + hitBox.h + addRect.h;
    int x2 = entity->x + entity->hitBox.x;
    int y2 = entity->y + entity->hitBox.y;
    int maxX2 = x2 + entity->hitBox.w;
    int maxY2 = y2 + entity->hitBox.h;

    if (maxX1 < x2 || x1 > maxX2) return false;
    if (maxY1 < y2 || y1 > maxY2) return false;

    return true;
}

void Entity::clearTexture() {
    SDL_DestroyTexture(imgTexture);
    imgTexture = nullptr;
}
