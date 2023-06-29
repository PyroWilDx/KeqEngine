//
// Created by pyrowildx on 13/05/23.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL2/SDL.h>

class WindowRenderer;

class Entity {

public:
    Entity(double x, double y);

    Entity(double x, double y, int w, int h, SDL_Texture *texture);

    virtual ~Entity();

    virtual void setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    void moveX();

    void moveY();

    virtual void fallGravity();

    void moveTo(double x_, double y_);

    void moveTo(Entity *entity);

    void moveTo(Entity *entity, double addX, double addY);

    virtual void getRealSize(double *pW, double *pH);

    virtual bool shouldTranslate();

    virtual SDL_Rect getRenderRect();

    bool collides(Entity *entity, SDL_Rect addRect) const;

    void clearTexture();

    inline void getHitbox(SDL_Rect collisionRect_) { hitbox = collisionRect_; }

    inline void setRenderWHMultiplier(double wMultiplier, double hMultiplier) {
        renderWMultiplier = wMultiplier;
        renderHMultiplier = hMultiplier;
    }

    inline void addRenderWHMultiplier(double addW, double addH, double maxW, double maxH) {
        renderWMultiplier += addW;
        if (renderWMultiplier > maxW) renderWMultiplier = maxW;
        renderHMultiplier += addH;
        if (renderHMultiplier > maxH) renderHMultiplier = maxH;
    }

    inline void setRotation(double rotation_) { rotation = rotation_; }

    [[nodiscard]] inline double getX() const { return x; }

    [[nodiscard]] inline double getY() const { return y; }

    [[nodiscard]] inline bool isFacingEast() const { return facingEast; }

    [[nodiscard]] inline SDL_Rect getFrame() const { return frame; }

    [[nodiscard]] inline SDL_Rect getHitbox() const { return hitbox; }

    [[nodiscard]] inline SDL_Texture *getTexture() const { return texture; }

    [[nodiscard]] inline double getRenderWMultiplier() const { return renderWMultiplier; }

    [[nodiscard]] inline double getRenderHMultiplier() const { return renderHMultiplier; }

    [[nodiscard]] inline double getRotation() const { return rotation; }

protected:
    double x, y;
    bool facingEast;
    double xVelocity, yVelocity;
    double weight;
    SDL_Rect frame;
    SDL_Rect hitbox;
    SDL_Texture *texture;
    double renderWMultiplier, renderHMultiplier;
    double rotation;

private:

};

#endif
