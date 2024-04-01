//
// Created by pyrow on 01/04/2024.
//

#include "EntityInfo/Equipment.hpp"

Equipment::Equipment(int maxLevel_) {
    maxLevel = maxLevel_;
    eLevel = 1;
}

bool Equipment::levelUp() {
    if (eLevel >= maxLevel) return false;

    eLevel++;
    onLevelUp();

    return true;
}

double Equipment::getLevelMultiplier() const {
    return (double) eLevel / maxLevel;
}
