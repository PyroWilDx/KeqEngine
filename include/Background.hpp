//
// Created by pyrowildx on 01/06/23.
//

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Entity.hpp"
#include "Keqing.hpp"

class Background : public Entity {

public:
    Background(int screenW, int screenH, int totalWidth, int totalHeight,
               SDL_Texture *texture);

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
