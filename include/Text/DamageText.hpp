//
// Created by pyrow on 09/02/2024.
//

#ifndef DAMAGETEXT_HPP
#define DAMAGETEXT_HPP

#include "Text/Text.hpp"

#define DT_DEFAULT_FONT_SIZE 16
#define DT_DEFAULT_AVG_FONT_SIZE 18
#define DT_DEFAULT_BIG_FONT_SIZE 20

#define DT_DEFAULT_DISPLAY_DURATION 760
#define DT_DEFAULT_AVG_DISPLAY_DURATION 900
#define DT_DEFAULT_LONG_DISPLAY_DURATION 1200

#define DT_BIG_RM 1.32

class DamageText : public Text {

public:
    DamageText(int atkDamage, SDL_Color *textColor, int fontSize,
               int dmgTextDuration);

    void moveToEntityCenterIgnoreRenderWH(Entity *centerEntity);

    bool onGameFrame() override;

private:
    int displayDuration;
    bool finishResize;

};

#endif
