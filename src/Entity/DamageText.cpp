//
// Created by pyrow on 09/02/2024.
//

#include "Entity/DamageText.hpp"
#include "Utils/Global.hpp"
#include "World/World.hpp"

DamageText::DamageText(int atkDamage, SDL_Color *textColor)
        : Text(std::to_string(atkDamage).c_str(), textColor, 16) {

}

DamageText::DamageText(int atkDamage)
        : DamageText(atkDamage, nullptr) {

}

bool DamageText::onGameFrame() {
    bool doNext = Entity::onGameFrame();

    if (doNext) {
        if (timeSinceCreation > DISPLAY_DURATION) {
            Global::gWorld->removeDamageText(this);
            return false;
        }
        y -= 0.01 * Global::dt;
    }

    return doNext;
}
