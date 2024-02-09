//
// Created by pyrow on 09/02/2024.
//

#ifndef DAMAGETEXT_HPP
#define DAMAGETEXT_HPP

#include "Entity/Text.hpp"

#define DISPLAY_DURATION 800

class DamageText : public Text {

public:
    DamageText(int atkDamage, SDL_Color *textColor);

    explicit DamageText(int atkDamage);

    bool onGameFrame() override;

private:

};

#endif
