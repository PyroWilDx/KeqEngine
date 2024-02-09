//
// Created by pyrowildx on 13/05/23.
//

#include <cmath>
#include "AbstractEntity/Entity.hpp"
#include "Utils/Global.hpp"
#include "WindowRenderer.hpp"
#include "World/World.hpp"
#include "Utils/Utils.hpp"
#include "World/Background.hpp"
#include "Utils/Colors.hpp"

Entity::Entity()
        : imgFrame({0, 0, 0, 0}), hitBox(imgFrame) {
    x = 0;
    y = 0;
    xVelocity = 0;
    yVelocity = 0;
    gravityWeight = 0;
    subjectToGravity = true;
    facingEast = true;
    imgTexture = nullptr;
    renderWMultiplier = 1;
    renderHMultiplier = 1;
    degRotation = 0;
    this->timeSinceCreation = 0;
}

Entity::Entity(double x, double y)
        : Entity() {
    this->x = x;
    this->y = y;
}

Entity::Entity(double x, double y, int frameW, int frameH)
        : Entity(x, y) {
    imgFrame.w = frameW;
    imgFrame.h = frameH;
}

Entity::Entity(double x, double y, int frameW, int frameH, const char *imgPath)
        : Entity(x, y, frameW, frameH) {
    hitBox = imgFrame;
    imgTexture = WindowRenderer::getInstance()->loadTexture(imgPath);
}

Entity::Entity(double x, double y, double renderWM, double renderHM)
        : Entity(x, y) {
    renderWMultiplier = renderWM;
    renderHMultiplier = renderHM;
}

Entity::Entity(double x, double y, int frameW, int frameH,
               double renderWM, double renderHM, const char *imgPath)
        : Entity(x, y, frameW, frameH, imgPath) {
    setRenderWHMultiplier(renderWM, renderHM);
}

Entity::~Entity() {
    clearTexture();

    Global::gWorld->onEntityRemove(this);
}

void Entity::setRGBAMod(Uint8 a) {
    SDL_SetTextureAlphaMod(imgTexture, a);
}

void Entity::setRGBAMod(Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetTextureColorMod(imgTexture, r, g, b);
}

void Entity::setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    Entity::setRGBAMod(r, g, b);
    Entity::setRGBAMod(a);
}

std::vector<double> *Entity::getXArrayToCheck() const {
    const int N = 4;
    auto *xToCheck = new std::vector<double>;
    double xLeft = x + (double) hitBox.x;
    xToCheck->push_back(xLeft + 1);
    const double tmpDiv = N - 1;
    for (int i = 1; i < N - 1; i++) {
        xToCheck->push_back(xLeft + (double) (hitBox.w * i) / tmpDiv);
    }
    xToCheck->push_back(xLeft + (double) (hitBox.w - 1));
    return xToCheck;
}

std::vector<double> *Entity::getYArrayToCheck() const {
    const int N = 8;
    auto *yToCheck = new std::vector<double>;
    double yUp = y + (double) hitBox.y;
    yToCheck->push_back(yUp + 1);
    const double tmpDiv = N - 1;
    for (int i = 1; i < N - 1; i++) {
        yToCheck->push_back(yUp + (double) (hitBox.h * i) / tmpDiv);
    }
    yToCheck->push_back(yUp + (double) (hitBox.h - 1));
    return yToCheck;
}

bool Entity::checkXCollision(bool checkRight) {
    bool res = false;

    World *gWorld = Global::gWorld;
    std::vector<double> *yToCheck = getYArrayToCheck();

    if (checkRight) {
        double xRight = x + hitBox.x + hitBox.w;
        int xWall = INT32_MAX;
        for (double vY: *yToCheck) {
            int xNearest = gWorld->getNearestWallFrom(xRight, vY,
                                                      KEY_Q);
            if (xNearest != GET_NEAREST_WALL_RETURN_NONE) xWall = std::min(xWall, xNearest);
        }
        if (xRight > xWall) {
            if (xWall != INT32_MAX) {
                x = xWall - hitBox.w - hitBox.x;
                res = true;
            }
        }

    } else {
        double xLeft = x + hitBox.x;
        int xWall = -INT32_MAX;
        for (double vY: *yToCheck) {
            int xNearest = gWorld->getNearestWallFrom(xLeft, vY,
                                                      KEY_D);
            if (xNearest != GET_NEAREST_WALL_RETURN_NONE) xWall = std::max(xWall, xNearest);
        }
        if (xLeft < xWall) {
            if (xWall != -INT32_MAX) {
                x = xWall - hitBox.x;
                res = true;
            }
        }
    }

    delete yToCheck;
    return res;
}

bool Entity::checkYCollision(bool checkDown) {
    bool res = false;

    World *gWorld = Global::gWorld;
    std::vector<double> *xToCheck = getXArrayToCheck();

    if (checkDown) {
        double yDown = y + hitBox.y + hitBox.h;
        int yWall = INT32_MAX;
        for (double vX: *xToCheck) {
            int yNearest = gWorld->getNearestWallFrom(vX, yDown,
                                                      KEY_Z);
            if (yNearest != GET_NEAREST_WALL_RETURN_NONE) yWall = std::min(yWall, yNearest);
        }
        if (yDown > yWall) {
            if (yWall != INT32_MAX) {
                y = yWall - hitBox.h - hitBox.y;
                res = true;
            }
        }

    } else {
        double yUp = y + hitBox.y;
        int yWall = -INT32_MAX;
        for (double vX: *xToCheck) {
            int yNearest = gWorld->getNearestWallFrom(vX, yUp,
                                                      KEY_S);
            if (yNearest != GET_NEAREST_WALL_RETURN_NONE) yWall = std::max(yWall, yNearest);
        }
        if (yUp < yWall) {
            if (yWall != -INT32_MAX) {
                y = yWall - hitBox.y;
                res = true;
            }
        }
    }

    delete xToCheck;
    return res;
}

void Entity::moveToNearestSurface() {
    double baseX = x;
    double baseY = y;
    double baseCenterX, baseCenterY;
    getSelfCenter(&baseCenterX, &baseCenterY);

    double newCenterX, newCenterY;

    auto bestDistance = DBL_MAX;
    double bestX = baseX;
    double bestY = baseY;

    for (bool checkRight: {false, true}) {
        moveTo(baseX, baseY);
        if (checkXCollision(checkRight)) {
            if (!isHittingWall()) {
                getSelfCenter(&newCenterX, &newCenterY);
                double currDistance = getDistance(baseCenterX, baseCenterY,
                                                  newCenterX, newCenterY);
                if (currDistance < bestDistance) {
                    bestDistance = currDistance;
                    bestX = x;
                    bestY = y;
                }
            }
        }
    }

    for (bool checkDown: {false, true}) {
        moveTo(baseX, baseY);
        if (checkYCollision(checkDown)) {
            if (!isHittingWall()) {
                getSelfCenter(&newCenterX, &newCenterY);
                double currDistance = getDistance(baseCenterX, baseCenterY,
                                                  newCenterX, newCenterY);
                if (currDistance < bestDistance) {
                    bestDistance = currDistance;
                    bestX = x;
                    bestY = y;
                }
            }
        }
    }

    for (bool checkRight: {false, true}) {
        for (bool checkDown: {false, true}) {
            if (checkXCollision(checkRight) || checkYCollision(checkDown)) {
                if (!isHittingWall()) {
                    getSelfCenter(&newCenterX, &newCenterY);
                    double currDistance = getDistance(baseCenterX, baseCenterY,
                                                      newCenterX, newCenterY);
                    if (currDistance < bestDistance) {
                        bestDistance = currDistance;
                        bestX = x;
                        bestY = y;
                    }
                }
            }

            if (checkYCollision(checkDown) || checkXCollision(checkRight)) {
                if (!isHittingWall()) {
                    getSelfCenter(&newCenterX, &newCenterY);
                    double currDistance = getDistance(baseCenterX, baseCenterY,
                                                      newCenterX, newCenterY);
                    if (currDistance < bestDistance) {
                        bestDistance = currDistance;
                        bestX = x;
                        bestY = y;
                    }
                }
            }
        }
    }
    moveTo(bestX, bestY);
}

void Entity::moveX() {
    double addX = moveXNoCheck();

    checkXCollision((addX > 0));
}

void Entity::moveY() {
    moveYNoCheck();

    checkYCollision((yVelocity > 0));
}

double Entity::moveXNoCheck() {
    if (xVelocity == 0) return 0;

    double addX = xVelocity * (double) Global::dt;

    if (!facingEast) addX = -addX;

    if (degRotation != 0) {
        addX = addX * cos(degreeToRad(degRotation));
    }

    x += addX;

    return addX;
}

double Entity::moveYNoCheck() {
    if (yVelocity == 0) return 0;

    double addY = yVelocity * (double) Global::dt;

    if (degRotation != 0) {
        addY = addY * -sin(degreeToRad(degRotation));
    }

    y += addY;

    return addY;
}

bool Entity::isHittingWallHorizontallySide(bool sideLeft) const {
    World *gWorld = Global::gWorld;

    double xCheck = x + (double) hitBox.x;
    if (!sideLeft) xCheck += (double) hitBox.w;
    std::vector<double> *yToCheck = getYArrayToCheck();

    bool res = false;
    for (double vY: *yToCheck) {
        if (gWorld->isPixelSurface(xCheck, vY)) {
            res = true;
            break;
        }
    }

    delete yToCheck;

    return res;
}

bool Entity::isHittingWallHorizontally() const {
    return (isHittingWallHorizontallySide(true) || isHittingWallHorizontallySide(false));
}

bool Entity::isHittingWallVerticallySide(bool sideUp) const {
    World *gWorld = Global::gWorld;

    double yCheck = y + hitBox.y;
    if (!sideUp) yCheck += hitBox.h;
    std::vector<double> *xToCheck = getXArrayToCheck();

    bool res = false;
    for (double vX: *xToCheck) {
        if (gWorld->isPixelSurface(vX, yCheck)) {
            res = true;
            break;
        }
    }

    delete xToCheck;

    return res;
}

bool Entity::isInAir() {
    return !isHittingWallVerticallySide(false);
}

bool Entity::isHittingCeiling() const {
    return isHittingWallVerticallySide(true);
}

bool Entity::isHittingWall() const {
    World *gWorld = Global::gWorld;

    std::vector<double> *xToCheck = getXArrayToCheck();
    std::vector<double> *yToCheck = getYArrayToCheck();

    bool res = false;
    for (double vX: *xToCheck) {
        for (double vY: *yToCheck) {
            if (gWorld->isPixelSurface(vX, vY)) {
                res = true;
                break;
            }
        }
    }

    delete xToCheck;
    delete yToCheck;

    return res;
}

double Entity::getFallGravityAddVelocity() const {
    return (gravityWeight * (double) Global::dt);
}

void Entity::fallGravity() {
    if (!subjectToGravity) return;

    if (isHittingCeiling()) {
        yVelocity = -yVelocity / 3.2;
    }

    if (isInAir()) {
        yVelocity += getFallGravityAddVelocity();
    } else {
        if (yVelocity > 0) {
            yVelocity = 0;
        }
    }
}

void Entity::cancelGravity(double factor) {
    if (isInAir()) {
        yVelocity -= getFallGravityAddVelocity() * factor;
    }
}

void Entity::moveTo(double x_, double y_) {
    x = x_;
    y = y_;
}

void Entity::moveToCenter(double x_, double y_) {
    double realW, realH;
    getRealSize(&realW, &realH);
    moveTo(x_ - realW / 2., y_ - realH / 2.);
}

void Entity::moveToDownLeft(double xLeft, double yDown) {
    double yUp;
    if (hitBox.w == 0 && hitBox.h == 0) {
        double realH;
        getRealSize(nullptr, &realH);
        yUp = yDown - realH;
    } else {
        yUp = yDown - hitBox.h;
    }
    xLeft -= hitBox.x;
    yUp -= hitBox.y;
    moveTo(xLeft, yUp);
}

void Entity::moveAdd(double addX, double addY) {
    x += addX;
    y += addY;
}

void Entity::centerHorizontal(double startX, double maxX, double newY) {
    int halfW = getW() / 2;
    x = startX + (maxX - startX) / 2 - halfW;
    y = newY;
}

void Entity::moveToScreenCenterHorizontal(double newY) {
    centerHorizontal(0, SCREEN_BASE_WIDTH, newY);
}

void Entity::moveToEntityBelow(Entity *entity, double yPadding) {
    centerHorizontal(entity->x, entity->x + entity->getW(),
                     entity->y + entity->getH() + yPadding);
}

double Entity::distTo(Entity *entity) const {
    double xDiff = entity->getX() - getX();
    double yDiff = entity->getY() - getY();
    return std::sqrt(xDiff * xDiff + yDiff * yDiff);
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
        SDL_Rect backgroundFrame = Global::gWorld->getBackground()->getFrame();
        dstX -= (double) backgroundFrame.x;
        dstY -= (double) backgroundFrame.y;
    }

    double xCoeff, yCoeff;
    getScreenXYCoeff(&xCoeff, &yCoeff);

    SDL_Rect dst = {(int) (dstX * xCoeff), (int) (dstY * yCoeff),
                    (int) (realW * xCoeff), (int) (realH * yCoeff)};

    shiftXYFromScreenPosition(&dst.x, &dst.y);

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

void Entity::addXYVelocity(double addXV, double addYV) {
    xVelocity += addXV;
    yVelocity += addYV;
}

void Entity::getSelfCenter(double *pX, double *pY) {
    if (hitBox.w != 0 && hitBox.h != 0) {
        *pX = getX() + getBaseHitBoxX() + getHalfBaseHitBoxW();
        *pY = getY() + getBaseHitBoxY() + getHalfBaseHitBoxH();
    } else {
        double realW, realH;
        getRealSize(&realW, &realH);
        *pX = getX() + realW / 2.;
        *pY = getY() + realH / 2.;
    }
}

void Entity::getToEntityCenterXY(Entity *centerEntity, double *pX, double *pY) {
    double realW, realH;
    if (hitBox.w != 0 && hitBox.h != 0) {
        realW = hitBox.w;
        realH = hitBox.h;
    } else {
        this->getRealSize(&realW, &realH);
    }

    double vX = centerEntity->getX();
    double vY = centerEntity->getY();
    double addX, addY, addW, addH;
    SDL_Rect collisionRect = centerEntity->getHitBox();
    if (collisionRect.w != 0 && collisionRect.h != 0) {
        addX = (double) centerEntity->getHitBox().x;
        addY = (double) centerEntity->getHitBox().y;
        addW = (double) centerEntity->getHitBox().w;
        addH = (double) centerEntity->getHitBox().h;
    } else {
        addX = 0;
        addY = 0;
        centerEntity->getRealSize(&addW, &addH);
    }

    if (hitBox.w != 0 && hitBox.h != 0) {
        addX -= hitBox.x;
        addY -= hitBox.y;
    }

    if (pX != nullptr) *pX = vX + addX + (addW / 2. - realW / 2.);
    if (pY != nullptr) *pY = vY + addY + (addH / 2. - realH / 2.);
}

void Entity::moveToEntityCenter(Entity *centerEntity, bool takeFaceEast) {
    if (takeFaceEast) facingEast = centerEntity->isFacingEast();
    getToEntityCenterXY(centerEntity, &x, &y);
}

void Entity::moveToEntityCenterIgnoreHitBox(Entity *centerEntity, bool takeFaceEast) {
    if (takeFaceEast) facingEast = centerEntity->isFacingEast();

    int hitBoxW = centerEntity->getHitBox().w;
    int hitBoxH = centerEntity->getHitBox().h;

    centerEntity->getHitBoxAddr()->w = 0;
    centerEntity->getHitBoxAddr()->h = 0;

    getToEntityCenterXY(centerEntity, &x, &y);

    centerEntity->getHitBoxAddr()->w = hitBoxW;
    centerEntity->getHitBoxAddr()->h = hitBoxH;
}

bool Entity::onGameFrame() {
    timeSinceCreation += Global::dt;

    return true;
}

void Entity::renderSelf(SDL_Renderer *gRenderer) {
    SDL_Rect src = imgFrame;

    if (src.x < 0 || src.y < 0) return;
    if (src.w <= 0 || src.h <= 0) return;

    SDL_Rect dst = getRenderRect();

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
}

void Entity::renderHitBox(SDL_Renderer *gRenderer) {
    SDL_Rect dstHitBox = hitBox;
    if (dstHitBox.w != 0 && dstHitBox.h != 0) {
        dstHitBox.x += (int) x;
        dstHitBox.y += (int) y;
        WindowRenderer::renderRect(&dstHitBox, false,
                                   COLOR_MAX, 0, 0, COLOR_MAX,
                                   gRenderer,
                                   false, true);
    }
}

void Entity::setHitBoxCentered(int hitBoxW, int hitBoxH) {
    double realW, realH;
    getRealSize(&realW, &realH);

    hitBox.x = (int) ((realW - (double) hitBoxW) / 2.);
    hitBox.y = (int) ((realH - (double) hitBoxH) / 2.);
    hitBox.w = hitBoxW;
    hitBox.h = hitBoxH;
}

double Entity::getBaseHitBoxX() const {
    return ((double) hitBox.x / renderWMultiplier);
}

double Entity::getBaseHitBoxY() const {
    return ((double) hitBox.y / renderHMultiplier);
}

double Entity::getBaseHitBoxW() const {
    return ((double) hitBox.w / renderWMultiplier);
}

double Entity::getBaseHitBoxH() const {
    return ((double) hitBox.h / renderHMultiplier);
}

double Entity::getHalfBaseHitBoxW() const {
    return getBaseHitBoxW() / 2.;
}

double Entity::getHalfBaseHitBoxH() const {
    return getBaseHitBoxH() / 2.;
}

bool Entity::hitBoxCollision(Entity *entity) const {
    double x1 = x + (double) hitBox.x;
    double y1 = y + (double) hitBox.y;
    double maxX1 = x1 + (double) hitBox.w;
    double maxY1 = y1 + (double) hitBox.h;
    double x2 = entity->x + (double) entity->hitBox.x;
    double y2 = entity->y + (double) entity->hitBox.y;
    double maxX2 = x2 + (double) entity->hitBox.w;
    double maxY2 = y2 + (double) entity->hitBox.h;

    if (maxX1 < x2 || x1 > maxX2) return false;
    if (maxY1 < y2 || y1 > maxY2) return false;

    return true;
}

void Entity::clearTexture() {
    if (imgTexture != nullptr) {
        SDL_DestroyTexture(imgTexture);
        imgTexture = nullptr;
    }
}
