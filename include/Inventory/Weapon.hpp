//
// Created by pyrow on 11/02/2024.
//

#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>
#include "Equipment.hpp"

#define WEAPON_MAX_LEVEL 100

#define DULL_BLADE "Dull Blade"
#define LION_ROAR "Lion's Roar"
#define MISTSPLITTER "Mistsplitter Reforged"

typedef struct WeaponInfo {
    int maxAtkFlat;
    double maxAtkMultiplier;
    double maxCritRate;
    double maxCritDamage;
    double wElMultiplier;
} WeaponInfo;

class Weapon : public Equipment {

public:
    explicit Weapon(const char *wNameCStr);

    void onLevelUp() override;

    SDL_Texture *getTextureFromName();

    Weapon *getUpgradedWeapon();

    [[nodiscard]] inline std::string getName() const { return wName; }

    [[nodiscard]] inline int getWAtkFlat() const { return wAtkFlat; }

    [[nodiscard]] inline double getWAtkMultiplier() const { return wAtkMultiplier; }

    [[nodiscard]] inline double getWCritRate() const { return wCritRate; }

    [[nodiscard]] inline double getWCritDamage() const { return wCritDamage; }

    [[nodiscard]] inline double getWElMultiplier() const { return wElMultiplier; }

    static std::unordered_map<std::string, WeaponInfo> gWeapons;

private:
    std::string wName;
    WeaponInfo wInfo;
    int wAtkFlat;
    double wAtkMultiplier;
    double wCritRate;
    double wCritDamage;
    double wElMultiplier;

};

#endif
