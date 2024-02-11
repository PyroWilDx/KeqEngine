//
// Created by pyrow on 11/02/2024.
//

#include <SDL2/SDL_log.h>
#include "EntityInfo/Weapon.hpp"

std::unordered_map<std::string, WeaponInfo> Weapon::gWeapons = {
        {DULL_BLADE,   {200, 0,
                               0, 0,     0.06}},
        {LION_ROAR,    {560, 0.413,
                               0, 0,     0.22}},
        {MISTSPLITTER, {730, 0,
                               0, 0.441, 0.32}}
};

Weapon::Weapon(const char *wNameCStr)
        : wInfo() {
    wName = std::string(wNameCStr);
    wInfo = Weapon::gWeapons[wName];
    wLevel = 1;

    double lM = getLevelMultiplier();
    wAtkFlat = (int) (lM * wInfo.maxAtkFlat);
    wAtkMultiplier = 1. + (lM * wInfo.maxAtkMultiplier);
    wCritRate = lM * wInfo.maxCritRate;
    wCritDamage = lM * wInfo.maxCritDamage;
    wElMultiplier = wInfo.wElMultiplier;
}

double Weapon::getLevelMultiplier() const {
    return (double) wLevel / WEAPON_MAX_LEVEL;
}

void Weapon::levelUpAndUpdateStats() {
    if (wLevel >= WEAPON_MAX_LEVEL) return;

    wLevel++;

    double lM = getLevelMultiplier();
    wAtkFlat = (int) (lM * wInfo.maxAtkFlat);
    wAtkMultiplier = 1. + (lM * wInfo.maxAtkMultiplier);
    wCritRate = lM * wInfo.maxCritRate;
    wCritDamage = lM * wInfo.maxCritDamage;
}
