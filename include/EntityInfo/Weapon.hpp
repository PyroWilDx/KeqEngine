//
// Created by pyrow on 11/02/2024.
//

#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <string>
#include <unordered_map>

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

class Weapon {

public:
    explicit Weapon(const char *wNameCStr);

    [[nodiscard]] double getLevelMultiplier() const;

    void levelUpAndUpdateStats();

    static std::unordered_map<std::string, WeaponInfo> gWeapons;

private:
    std::string wName;
    WeaponInfo wInfo;
    int wLevel;
    int wAtkFlat;
    double wAtkMultiplier;
    double wCritRate;
    double wCritDamage;
    double wElMultiplier;

};

#endif
