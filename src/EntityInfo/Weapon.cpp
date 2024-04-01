//
// Created by pyrow on 11/02/2024.
//

#include <cmath>
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
        : Equipment(WEAPON_MAX_LEVEL),
          wInfo() {
    wName = std::string(wNameCStr);
    wInfo = Weapon::gWeapons[wName];

    double lM = getLevelMultiplier();
    wAtkFlat = (int) std::round(lM * wInfo.maxAtkFlat);
    wAtkMultiplier = lM * wInfo.maxAtkMultiplier;
    wCritRate = lM * wInfo.maxCritRate;
    wCritDamage = lM * wInfo.maxCritDamage;
    wElMultiplier = wInfo.wElMultiplier;
}

void Weapon::onLevelUp() {
    double lM = getLevelMultiplier();
    wAtkFlat = (int) std::round(lM * wInfo.maxAtkFlat);
    wAtkMultiplier = lM * wInfo.maxAtkMultiplier;
    wCritRate = lM * wInfo.maxCritRate;
    wCritDamage = lM * wInfo.maxCritDamage;
}
