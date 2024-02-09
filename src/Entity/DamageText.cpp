//
// Created by pyrow on 09/02/2024.
//

#include "Entity/DamageText.hpp"
#include "Utils/Global.hpp"
#include "World/World.hpp"
#include "Utils/Colors.hpp"

DamageText::DamageText(int atkDamage, SDL_Color *textColor, int fontSize,
                       int dmgTextDuration)
        : Text(std::to_string(atkDamage).c_str(), textColor, fontSize,
               &Colors::dColorBlack, 2) {
    setRenderWHMultiplier(BIG_RM, BIG_RM);

    this->displayDuration = dmgTextDuration;
    this->finishResize = false;
}

DamageText::DamageText(int atkDamage, SDL_Color *textColor)
        : DamageText(atkDamage, textColor, DEFAULT_BIG_FONT_SIZE,
                     DEFAULT_DISPLAY_DURATION) {

}

DamageText::DamageText(int atkDamage)
        : DamageText(atkDamage, nullptr, DEFAULT_FONT_SIZE,
                     DEFAULT_DISPLAY_DURATION) {

}

bool DamageText::onGameFrame() {
    bool doNext = Entity::onGameFrame();

    if (doNext) {
        if (timeSinceCreation > displayDuration) {
            Global::gWorld->popFrontDamageText();
            return false;
        }
        if (timeSinceCreation > displayDuration - COLOR_MAX / 2) {
            setRGBAMod((displayDuration - timeSinceCreation) * 2);
        }
        y -= 0.016 * Global::dt;
        if (!finishResize) {
            renderWMultiplier = renderWMultiplier - 0.0036 * Global::dt;
            renderHMultiplier = renderWMultiplier;
            if (renderWMultiplier < 1.) {
                renderWMultiplier = 1.;
                renderHMultiplier = 1.;
                finishResize = true;
            }
        }
    }

    return doNext;
}
