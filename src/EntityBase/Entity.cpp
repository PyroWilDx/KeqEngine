//
// Created by pyrowildx on 13/05/23.
//

#include "EntityBase/Entity.hpp"
#include "Utils/Global.hpp"

Entity::Entity()
        : imgFrame({0, 0, 0, 0}), hitBox(imgFrame) {
    x = 0;
    y = 0;
    xVelocity = 0;
    yVelocity = 0;
    gravityWeight = 0;
    facingEast = true;
    imgTexture = nullptr;
    renderWMultiplier = 1;
    renderHMultiplier = 1;
    degRotation = 0;
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
}

void Entity::setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetTextureColorMod(imgTexture, r, g, b);
    SDL_SetTextureAlphaMod(imgTexture, a);
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

void Entity::checkXCollision(bool checkRight) {
    World *gWorld = Global::currentWorld;
    std::vector<double> *yToCheck = getYArrayToCheck();

    if (checkRight) {
        double xRight = x + hitBox.x + hitBox.w;
        double xWall = INT32_MAX;
        for (double vY: *yToCheck) {
            xWall = std::min(xWall, gWorld->getNearestWallFrom(
                    xRight, vY, KEY_Q));
        }
        if (xRight > xWall) {
            x = xWall - hitBox.w - hitBox.x;
        }

    } else {
        double xLeft = x + hitBox.x;
        double xWall = -INT32_MAX;
        for (double vY: *yToCheck) {
            xWall = std::max(xWall, gWorld->getNearestWallFrom(
                    xLeft, vY, KEY_D));
        }
        if (xLeft < xWall) {
            x = xWall - hitBox.x;
        }
    }

    delete yToCheck;
}

void Entity::checkYCollision(bool checkDown) {
    World *gWorld = Global::currentWorld;
    std::vector<double> *xToCheck = getXArrayToCheck();

    if (checkDown) {
        double yDown = y + hitBox.y + hitBox.h;
        double yWall = INT32_MAX;
        for (double vX: *xToCheck) {
            yWall = std::min(yWall, gWorld->getNearestWallFrom(
                    vX, yDown, KEY_Z));
        }
        if (yDown > yWall) {
            y = yWall - hitBox.h - hitBox.y;
        }

    } else {
        double yUp = y + hitBox.y;
        double yWall = -INT32_MAX;
        for (double vX: *xToCheck) {
            yWall = std::max(yWall, gWorld->getNearestWallFrom(
                    vX, yUp, KEY_S));
        }
        if (yUp < yWall) {
            y = yWall - hitBox.y;
            yVelocity = 0;
        }
    }

    delete xToCheck;
}

void Entity::moveX() {
    double addX = xVelocity * (double) Global::dt;

    if (addX == 0) return;

    if (!facingEast) addX = -addX;
    x += addX;

    checkXCollision((addX > 0));
}

void Entity::moveY() {
    if (yVelocity == 0) return;

    y += yVelocity * (double) Global::dt;

    checkYCollision((yVelocity > 0));
}

bool Entity::isHittingWallHorizontallySide(bool sideLeft) const {
    World *gWorld = Global::currentWorld;

    double xCheck = x + (double) hitBox.x;
    if (!sideLeft) xCheck += (double) hitBox.w;
    std::vector<double> *yToCheck = getYArrayToCheck();

    for (double vY: *yToCheck) {
        if (gWorld->isPixelSurface(xCheck, vY)) return true;
    }

    return false;
}

bool Entity::isHittingWallHorizontally() const {
    return (isHittingWallHorizontallySide(true) || isHittingWallHorizontallySide(false));
}

bool Entity::isHittingWallVerticallySide(bool sideUp) const {
    World *gWorld = Global::currentWorld;

    double yCheck = y + hitBox.y;
    if (!sideUp) yCheck += hitBox.h;
    std::vector<double> *xToCheck = getXArrayToCheck();

    for (double vX: *xToCheck) {
        if (gWorld->isPixelSurface(vX, yCheck)) return true;
    }

    return false;
}

bool Entity::isInAir() const {
    return !isHittingWallVerticallySide(false);
}

bool Entity::isHittingCeiling() const {
    return isHittingWallVerticallySide(true);
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

void Entity::moveToCenter(double x_, double y_) {
    double realW, realH;
    getRealSize(&realW, &realH);
    moveTo(x_ - realW / 2.0, y_ - realH / 2.0);
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
    xLeft -= (hitBox.x - x);
    yUp -= (hitBox.y - y);
    moveTo(xLeft, yUp);
}

void Entity::moveAdd(double addX, double addY) {
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

void Entity::getSelfCenter(double *pX, double *pY) {
    if (hitBox.w != 0 && hitBox.h != 0) {
        *pX = x + hitBox.x + (double) hitBox.w / 2.0;
        *pY = y + hitBox.y + (double) hitBox.h / 2.0;
    } else {
        double realW, realH;
        getRealSize(&realW, &realH);
        *pX = x + realW / 2.0;
        *pY = y + realH / 2.0;
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

    *pX = vX + addX + (addW / 2.0 - (double) realW / 2.0);
    *pY = vY + addY + (addH / 2.0 - (double) realH / 2.0);
}

void Entity::moveToEntityCenter(Entity *centerEntity, bool takeFaceEast) {
    if (takeFaceEast) facingEast = centerEntity->isFacingEast();
    getToEntityCenterXY(centerEntity, &x, &y);
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
}

void Entity::renderHitBox(SDL_Renderer *gRenderer) {
    SDL_Rect dstHitBox = hitBox;
    if (dstHitBox.w != 0 && dstHitBox.h != 0) {
        dstHitBox.x += getX() - Global::currentWorld->getBackground()->getFrame().x;
        dstHitBox.y += getY() - Global::currentWorld->getBackground()->getFrame().y;
        double xCoeff, yCoeff;
        getScreenXYCoeff(&xCoeff, &yCoeff);
        dstHitBox.x = (int) ((double) dstHitBox.x * xCoeff);
        dstHitBox.y = (int) ((double) dstHitBox.y * yCoeff);
        dstHitBox.w = (int) ((double) dstHitBox.w * xCoeff);
        dstHitBox.h = (int) ((double) dstHitBox.h * yCoeff);
        shiftXYFromScreenPosition(&dstHitBox.x, &dstHitBox.y);
        SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(gRenderer, &dstHitBox);
    }
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
    return (getBaseHitBoxW() / 2.0);
}

double Entity::getHalfBaseHitBoxH() const {
    return (getBaseHitBoxH() / 2.0);
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
