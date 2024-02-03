//
// Created by pyrowildx on 25/06/2023.
//

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cmath>
#include "AbstractEntity/WorldEntity.hpp"

enum {
    BLOCK_WALL_INVISIBLE = 0,
    BLOCK_DIRT,
    BLOCK_GRASS,
    BLOCK_SNOW,
    BLOCK_QUARTZ,
    BLOCK_TNT,
    BLOCK_ENUM_N
};

class Block : public WorldEntity {

public:
    Block(int blockCode, double x, double y, int renderW, int renderH);

    void resizeToRenderSize() override;

    void renderSelf(SDL_Renderer *gRenderer) override;

    [[nodiscard]] inline int getCode() const { return blockCode; }

private:
    void setBlockInfo();

    int blockCode;
    bool matchRenderSize;

};

#endif
