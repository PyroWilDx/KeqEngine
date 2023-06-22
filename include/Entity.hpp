//
// Created by pyrowildx on 13/05/23.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL2/SDL.h>

class WindowRenderer;

class Entity {

public:
    Entity(float x, float y);

    Entity(float x, float y, int w, int h, SDL_Texture *texture);

    virtual ~Entity();

    virtual void setRGBAMod(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

    virtual void renderSelf(WindowRenderer *window);

    virtual void move(int dt);

    void moveTo(float x_, float y_);

    void moveTo(Entity *entity);

    void moveTo(Entity *entity, float addX, float addY);

    virtual void getRealSize(float *pW, float *pH);

    bool collides(Entity *entity, SDL_Rect addRect) const;

    void clearTexture();

    inline void setCollisionRect(SDL_Rect collisionRect_) { collisionRect = collisionRect_; }

    inline void setRenderWHMultiplier(float wMultiplier, float hMultiplier) {
        renderWMultiplier = wMultiplier;
        renderHMultiplier = hMultiplier;
    }

    inline void addRenderWHMultiplier(float addW, float addH, float maxW, float maxH) {
        renderWMultiplier += addW;
        if (renderWMultiplier > maxW) renderWMultiplier = maxW;
        renderHMultiplier += addH;
        if (renderHMultiplier > maxH) renderHMultiplier = maxH;
    }

    inline void setRotation(double rotation_) { rotation = rotation_; }

    inline void setXYShift(int xShift_, int yShift_, int xShiftR_) {
        xShift = xShift_;
        yShift = yShift_;
        xShiftR = xShiftR_;
    }

    [[nodiscard]] inline float getX() const { return x; }

    [[nodiscard]] inline float getY() const { return y; }

    [[nodiscard]] inline float getXVelocity() const { return xVelocity; }

    [[nodiscard]] inline bool isFacingEast() const { return facingEast; }

    [[nodiscard]] inline SDL_Rect getFrame() const { return frame; }

    [[nodiscard]] inline SDL_Rect getCollisionRect() const { return collisionRect; }

    [[nodiscard]] inline SDL_Texture *getTexture() const { return texture; }

    [[nodiscard]] inline float getRenderWMultiplier() const { return renderWMultiplier; }

    [[nodiscard]] inline float getRenderHMultiplier() const { return renderHMultiplier; }

    [[nodiscard]] inline int getXShift() const { return xShift; }

    [[nodiscard]] inline int getYShift() const { return yShift; }

    [[nodiscard]] inline int getXShiftR() const { return xShiftR; }

    [[nodiscard]] inline double getRotation() const { return rotation; }

protected:
    float x, y;
    bool facingEast;
    float xVelocity, yVelocity;
    SDL_Rect frame;
    SDL_Rect collisionRect;
    SDL_Texture *texture;
    float renderWMultiplier, renderHMultiplier;
    int xShift, yShift, xShiftR;
    double rotation;

private:

};

#endif
