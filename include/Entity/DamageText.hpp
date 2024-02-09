//
// Created by pyrow on 09/02/2024.
//

#ifndef DAMAGETEXT_HPP
#define DAMAGETEXT_HPP

#include "Entity/Text.hpp"

#define DEFAULT_BIG_FONT_SIZE 20
#define DEFAULT_FONT_SIZE 16

#define BIG_RM 1.32
#define DEFAULT_DISPLAY_DURATION 760

class DamageText : public Text {

public:
    DamageText(int atkDamage, SDL_Color *textColor, int fontSize,
               int dmgTextDuration);

    DamageText(int atkDamage, SDL_Color *textColor);

    explicit DamageText(int atkDamage);

    bool onGameFrame() override;

private:
    int displayDuration;
    bool finishResize;

};

#endif
