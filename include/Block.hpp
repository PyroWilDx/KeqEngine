//
// Created by pyrow on 25/06/2023.
//

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cmath>
#include "Entity.hpp"
#include "Utils.hpp"

enum {
    BLOCK_NULL = 0,
    BLOCK_WALL_INVISIBLE,
    BLOCK_DIRT,
    BLOCK_ENUM_N
};

class Block : public Entity {

public:
    Block(int blockCode, double x, double y, int renderW, int renderH);

    void getRealSize(double *pW, double *pH) override;

//    SDL_Rect getRenderRect() override;

    inline int getX() { return roundToInt(x); }

    inline int getY() { return roundToInt(y); }

    [[nodiscard]] inline int getCode() const { return code; }

    [[nodiscard]] inline int getRenderW() const { return renderW; }

    [[nodiscard]] inline int getRenderH() const { return renderH; }

private:
    void setBlockInfo();

    int code;
    int renderW, renderH;

};

#endif
