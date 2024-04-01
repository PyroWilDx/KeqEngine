//
// Created by pyrow on 11/02/2024.
//

#include <cmath>
#include "EntityInfo/Weapon.hpp"
#include "WindowRenderer.hpp"

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

SDL_Texture *Weapon::getTextureFromName() {
    WindowRenderer *gWindow = WindowRenderer::getInstance();

    if (wName == DULL_BLADE) return gWindow->loadTexture("res/gfx/inventory/WDullBlade.png");
    else if (wName == LION_ROAR) return gWindow->loadTexture("res/gfx/inventory/WLionRoar.png");
    else if (wName == MISTSPLITTER) return gWindow->loadTexture("res/gfx/inventory/WMistsplitter.png");

    return nullptr;
}

Weapon *Weapon::getUpgradedWeapon() {
    Weapon *upgradedWeapon = nullptr;
    if (wName == DULL_BLADE) upgradedWeapon = new Weapon(LION_ROAR);
    else if (wName == LION_ROAR) upgradedWeapon = new Weapon(MISTSPLITTER);

    if (upgradedWeapon != nullptr) upgradedWeapon->setLevel(getLevel());

    return upgradedWeapon;
}
