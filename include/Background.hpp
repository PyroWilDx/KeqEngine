//
// Created by pyrowildx on 01/06/23.
//

#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "Entity.hpp"
#include "Player.hpp"

const float BACKGROUND_SPEED = 0.46f;

class Background : public Entity {

public:
    Background(int w, int h, int totalImgWidth, SDL_Texture *texture);

    void move(int dt, int xDirection);

    void destroy() override;

private:
    int totalImgWidth;

};

#endif
