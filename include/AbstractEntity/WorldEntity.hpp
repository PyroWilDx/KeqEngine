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

    void setHitBoxAuto();

    virtual void resizeToRenderSize();

    void moveTo(double x_, double y_) override;

    void getRealSize(double *pW, double *pH) override;

    void addImage(const char *imgPath, int frameW = 0, int frameH = 0);

    bool isPixelInSelfRect(double pixelX, double pixelY);

    bool getCollisionArea(SDL_Rect *collRect, SDL_Rect *resultRect);

    bool getCollisionArea(WorldEntity *worldEntity, SDL_Rect *resultRect);

    inline void setRenderW(int renderW_) { this->renderW = renderW_; }

    inline void setRenderH(int renderH_) { this->renderH = renderH_; }

    [[nodiscard]] inline int getW() const override {
        return getRenderW();
    }

    [[nodiscard]] inline int getH() const override {
        return getRenderH();
    }

    [[nodiscard]] inline int getX() { return (int) x; }

    [[nodiscard]] inline int getY() { return (int) y; }

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
