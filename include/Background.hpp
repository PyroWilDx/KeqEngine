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

    void translate(Entity *entity);

    void destroy() override;

private:
    int totalImgWidth;

};

#endif
