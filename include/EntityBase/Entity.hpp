//
// Created by pyrowildx on 13/05/23.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP

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

    virtual void setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    void checkXCollision(bool checkRight);

    virtual void moveX();

    void checkYCollision(bool checkDown);

    void moveY();

    bool isInAir() const;

    virtual void fallGravity();

    void moveTo(double x_, double y_);

    void moveTo(Entity *entity);

    void moveTo(Entity *entity, double addX, double addY);

    virtual void getRealSize(double *pW, double *pH);

    virtual bool shouldTranslate();

    virtual SDL_Rect getRenderRect();

    void addRenderWHMultiplier(double addW, double addH, double maxW, double maxH);

    void addRenderWHMultiplierR(double addW, double addH, double maxW, double maxH);

    void getToEntityCenterXY(Entity *centerEntity, double *pX, double *pY);

    void moveToEntityCenter(Entity *centerEntity);

    virtual void renderSelf(SDL_Renderer *gRenderer);

    bool collides(Entity *entity, SDL_Rect addRect) const;

    void clearTexture();

    inline void getHitbox(SDL_Rect collisionRect_) { hitBox = collisionRect_; }

    inline void setRenderWHMultiplier(double wMultiplier, double hMultiplier) {
        renderWMultiplier = wMultiplier;
        renderHMultiplier = hMultiplier;
    }

    inline void setRotation(double rotation_) { degRotation = rotation_; }

    [[nodiscard]] inline double getX() const { return x; }

    [[nodiscard]] inline double getY() const { return y; }

    [[nodiscard]] inline bool isFacingEast() const { return facingEast; }

    [[nodiscard]] inline double getYVelocity() const { return yVelocity; }

    [[nodiscard]] inline SDL_Rect getFrame() const { return imgFrame; }

    [[nodiscard]] inline SDL_Rect getHitBox() const { return hitBox; }

    [[nodiscard]] inline SDL_Texture *getTexture() const { return imgTexture; }

    [[nodiscard]] inline double getRenderWMultiplier() const { return renderWMultiplier; }

    [[nodiscard]] inline double getRenderHMultiplier() const { return renderHMultiplier; }

    [[nodiscard]] inline double getRotation() const { return degRotation; }

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

private:

};

#endif
