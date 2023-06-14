//
// Created by pyrowildx on 14/06/23.
//

#ifndef HUD_HPP
#define HUD_HPP

#include "Entity.hpp"

class Hud : public Entity {

public:
    explicit Hud(int x, int y, int w, int h, int frameW, SDL_Texture *texture);

    void moveToHudFrameCenter(Hud *centerHud, int whichFrame);

private:
    int frameW;

};

#endif
