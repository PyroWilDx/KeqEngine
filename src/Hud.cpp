//
// Created by pyrowildx on 14/06/23.
//

#include "Hud.hpp"

Hud::Hud(int x, int y, int w, int h, int frameW, SDL_Texture *texture) :
        Entity(x, y, w, h, texture) {
    this->frameW = frameW;
}

void Hud::moveToHudFrameCenter(Hud *centerHud, int whichFrame) {
    x = centerHud->x;
    y = centerHud->y;

    x += (int) ((float) whichFrame *
                (float) centerHud->frameW * centerHud->renderWMultiplier);
    x += (int) (((float) centerHud->frameW * centerHud->renderWMultiplier -
                 (float) frame.w * renderWMultiplier) / 2.0f);
    y += (int) (((float) centerHud->frame.h * centerHud->renderHMultiplier -
                 (float) frame.h * renderHMultiplier) / 2.0f);
}
