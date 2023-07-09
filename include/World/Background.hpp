//
// Created by pyrowildx on 01/06/23.
//

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "EntityBase/WorldEntity.hpp"

class Background : public WorldEntity {

public:
    Background(int renderW, int renderH, int totalWidth, int totalHeight,
               const char *imgPath);

    ~Background() override = default;

    bool shouldTranslate() override;

    void translate(Entity *entity);

    [[nodiscard]] int getTotalPixels() const;

    [[nodiscard]] inline int getTotalW() const { return totalWidth; }

    [[nodiscard]] inline int getTotalH() const { return totalHeight; }

private:
    int totalWidth;
    int totalHeight;

};

#endif
