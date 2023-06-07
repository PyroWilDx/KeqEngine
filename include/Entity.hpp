//
// Created by pyrowildx on 13/05/23.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL2/SDL.h>

#define DEFAULT_ENTITY_LENGTH 4

class Entity {

public:
    Entity(int x, int y, int z);

    Entity(int x, int y, int z, int w, int h, bool hasShadow, SDL_Texture *texture);

    void addX(int x_);

    virtual void move(int dt);

    void moveTo(int x_, int y_, int z_);

    bool collides(Entity *entity, SDL_Rect addRect);

    void clearTexture();

    virtual void destroy();

    inline void setFrame(SDL_Rect frame_) { frame = frame_; }

    inline void setCollisionRect(SDL_Rect collisionRect_) { collisionRect = collisionRect_; }

    inline void setRenderWHMultiplier(float wMultiplier, float hMultiplier) {
        renderWMultiplier = wMultiplier;
        renderHMultiplier = hMultiplier;
    }

    inline int getX() { return x; }

    inline int getY() { return y; }

    inline int getZ() { return z; }

    inline int getXDirection() { return xDirection; }

    inline SDL_Rect getFrame() { return frame; }

    inline SDL_Rect getCollisionRect() { return collisionRect; }

    inline bool getHasShadow() { return hasShadow; }

    inline SDL_Texture *getTexture() { return texture; }

    inline float getRenderWMultiplier() { return renderWMultiplier; }

    inline float getRenderHMultiplier() { return renderHMultiplier; }

    inline int getXShift() { return xShift; }

    inline int getYShift() { return yShift; }

protected:
    int x, y, z;
    int xDirection, yDirection, zDirection;
    float speed;
    SDL_Rect frame;
    SDL_Rect collisionRect;
    bool hasShadow;
    SDL_Texture *texture;
    float renderWMultiplier, renderHMultiplier;
    int xShift, yShift;

private:

};

#endif
