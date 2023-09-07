//
// Created by pyrowildx on 09/07/2023.
//

#ifndef WORLDENTITY_HPP
#define WORLDENTITY_HPP

#include "Entity.hpp"

typedef enum {
    WORLD_BACKGROUND = 0,
    WORLD_BLOCK,
    WORLD_BUTTON,
    WORLD_ENUM_N
} WorldEntityType;

class WorldEntity : public Entity {

public:
    WorldEntity(double x, double y, int renderW, int renderH, WorldEntityType worldEntityType);

    void getRealSize(double *pW, double *pH) override;

    void addImage(const char *imgPath, int frameW = 0, int frameH = 0);

    bool isPixelInSelfRect(double pixelX, double pixelY);

    inline int getX() { return (int) x; }

    inline int getY() { return (int) y; }

    [[nodiscard]] inline int getWorldEntityType() const { return worldEntityType; }

    [[nodiscard]] inline int getWorldCode() const { return worldCode; }

    [[nodiscard]] inline int getRenderW() const { return renderW; }

    [[nodiscard]] inline int getRenderH() const { return renderH; }

private:
    static int entityCount;

    WorldEntityType worldEntityType;
    int worldCode;
    int renderW, renderH;

};

#endif
