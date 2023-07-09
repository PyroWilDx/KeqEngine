//
// Created by pyrow on 25/06/2023.
//

#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <cmath>
#include "EntityBase/WorldEntity.hpp"
#include "Utils/Utils.hpp"

enum {
    BLOCK_WALL_INVISIBLE = 0,
    BLOCK_DIRT,
    BLOCK_ENUM_N
};

class Block : public WorldEntity {

public:
    Block(int blockCode, double x, double y, int renderW, int renderH);

    void renderSelf(SDL_Renderer *gRenderer) override;

    [[nodiscard]] inline int getCode() const { return blockCode; }

private:
    void setBlockInfo();

    int blockCode;

};

#endif
