//
// Created by pyrowildx on 13/05/23.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <vector>
#include <SDL2/SDL.h>

class WindowRenderer;

class Entity {

public:
    Entity();

    Entity(double x, double y);

    Entity(double x, double y, int frameW, int frameH);

    Entity(double x, double y, int frameW, int frameH, const char *imgPath);

    Entity(double x, double y, double renderWM, double renderHM);

    Entity(double x, double y, int frameW, int frameH,
           double renderWM, double renderHM, const char *imgPath);

    virtual ~Entity();

    virtual void setRGBAMod(Uint8 a);

    virtual void setRGBAMod(Uint8 r, Uint8 g, Uint8 b);

    virtual void setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    [[nodiscard]] std::vector<double> *getXArrayToCheck() const;

    [[nodiscard]] std::vector<double> *getYArrayToCheck() const;

    bool checkXCollision(bool checkRight);

    bool checkYCollision(bool checkDown);

    void findNearestSurface();

    virtual void moveX();

    void moveY();

    [[nodiscard]] bool isHittingWallHorizontallySide(bool sideLeft) const;

    [[nodiscard]] bool isHittingWallHorizontally() const;

    [[nodiscard]] bool isHittingWallVerticallySide(bool sideUp) const;

    [[nodiscard]] virtual bool isInAir();

    [[nodiscard]] bool isHittingCeiling() const;

    [[nodiscard]] bool isHittingWall() const;

    [[nodiscard]] double getFallGravityAddVelocity() const;

    virtual void fallGravity();

    void cancelGravity(double factor = 1);

    void moveTo(double x_, double y_);

    void moveToCenter(double x_, double y_);

    void moveToDownLeft(double xLeft, double yDown);

    void moveAdd(double addX, double addY);

    virtual void getRealSize(double *pW, double *pH);

    virtual bool shouldTranslate();

    virtual SDL_Rect getRenderRect();

    void addRenderWHMultiplier(double addW, double addH, double maxW, double maxH);

    void addRenderWHMultiplierR(double addW, double addH, double maxW, double maxH);

    void getSelfCenter(double *pX, double *pY);

    void getToEntityCenterXY(Entity *centerEntity, double *pX, double *pY);

    void moveToEntityCenter(Entity *centerEntity, bool takeFaceEast = true);

    virtual void onGameFrame();

    virtual void renderSelf(SDL_Renderer *gRenderer);

    void renderHitBox(SDL_Renderer *gRenderer);

    void setHitBoxCentered(int hitBoxW, int hitBoxH);

    [[nodiscard]] double getBaseHitBoxX() const;

    [[nodiscard]] double getBaseHitBoxY() const;

    [[nodiscard]] double getBaseHitBoxW() const;

    [[nodiscard]] double getBaseHitBoxH() const;

    [[nodiscard]] double getHalfBaseHitBoxW() const;

    [[nodiscard]] double getHalfBaseHitBoxH() const;

    bool hitBoxCollision(Entity *entity) const;

    void clearTexture();

    inline void setY(double y_) { y = y_; }

    virtual inline void setFacingEast(bool facingEast_) { facingEast = facingEast_; }

    inline void setYVelocity(double yVelocity_) { yVelocity = yVelocity_; }

    inline void setXYVelocity(double xVelocity_, double yVelocity_) {
        xVelocity = xVelocity_;
        yVelocity = yVelocity_;
    }

    inline void setHitBox(SDL_Rect hitBox_) { hitBox = hitBox_; }

    inline void setTexture(SDL_Texture *imgTexture_, int frameW = 0, int frameH = 0) {
        imgTexture = imgTexture_;
        if (frameW != 0 && frameH != 0) {
            imgFrame.w = frameW;
            imgFrame.h = frameH;
        } else {
            SDL_QueryTexture(imgTexture, nullptr, nullptr, &imgFrame.w, &imgFrame.h);
        }
    }

    inline void setRenderWHMultiplier(double wMultiplier, double hMultiplier) {
        renderWMultiplier = wMultiplier;
        renderHMultiplier = hMultiplier;
        if (hitBox.w != 0) {
            hitBox.x = (int) ((double) hitBox.x * wMultiplier);
            hitBox.w = (int) ((double) hitBox.w * wMultiplier);
        }
        if (hitBox.h != 0) {
            hitBox.y = (int) ((double) hitBox.y * hMultiplier);
            hitBox.h = (int) ((double) hitBox.h * hMultiplier);
        }
    }

    inline void setRotation(double rotation_) { degRotation = rotation_; }

    [[nodiscard]] inline double getX() const { return x; }

    [[nodiscard]] inline double getY() const { return y; }

    [[nodiscard]] inline bool isFacingEast() const { return facingEast; }

    [[nodiscard]] inline double getXVelocity() const { return xVelocity; }

    [[nodiscard]] inline double getYVelocity() const { return yVelocity; }

    [[nodiscard]] inline SDL_Rect getFrame() const { return imgFrame; }

    [[nodiscard]] inline SDL_Rect getHitBox() const { return hitBox; }

    [[nodiscard]] inline SDL_Texture *getTexture() const { return imgTexture; }

    [[nodiscard]] inline double getRenderWMultiplier() const { return renderWMultiplier; }

    [[nodiscard]] inline double getRenderHMultiplier() const { return renderHMultiplier; }

    [[nodiscard]] inline double getRotation() const { return degRotation; }

    [[nodiscard]] inline int getTimeSinceCreation() const { return timeSinceCreation; }

protected:
    double x, y;
    bool facingEast;
    double xVelocity, yVelocity;
    double gravityWeight;
    SDL_Rect imgFrame;
    SDL_Rect hitBox;
    SDL_Texture *imgTexture;
    double renderWMultiplier, renderHMultiplier;
    double degRotation;
    int timeSinceCreation;

private:

};

#endif
