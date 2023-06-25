//
// Created by pyrowildx on 01/06/23.
//

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Entity.hpp"
#include "Keqing.hpp"

class Background : public Entity {

public:
    Background(int w, int h, int totalImgWidth, SDL_Texture *texture);

    ~Background() override = default;

    bool shouldTranslate() override;

    void translate(Entity *entity);

private:
    int totalImgWidth;

};

#endif
