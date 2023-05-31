//
// Created by pyrowildx on 13/05/23.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SDL2/SDL.h>

class Entity {

public:
    Entity(int x, int y, int z);

    Entity(int x, int y, int z, int w, int h, SDL_Texture *texture);

    void moveTo(int x_, int y_, int z_);

    void clearTexture();

    virtual void destroy();

    inline void setFrameX(int x_) { frame.x = x_; }

    inline void setFrameY(int y_) { frame.y = y_; }

    inline void setFrameW(int w_) { frame.w = w_; }

    inline void setFrameH(int h_) { frame.h = h_; }

    inline void setRenderWH(int w_, int h_) {
        renderW = w_;
        renderH = h_;
    }

    inline int getX() { return x; }

    inline int getY() { return y; }

    inline int getZ() { return z; }

    inline int getXDirection() { return xDirection; }

    inline SDL_Rect getFrame() { return frame; }

    inline SDL_Texture *getTexture() { return texture; }

    inline int getRenderW() { return renderW; }

    inline int getRenderH() { return renderH; }

protected:
    int x, y, z;
    int xDirection, yDirection, zDirection;
    SDL_Rect frame;
    SDL_Texture *texture;
    int renderW, renderH;

private:

};

#endif
