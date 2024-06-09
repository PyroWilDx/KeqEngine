//
// Created by pyrow on 11/02/2024.
//

#include <algorithm>
#include "Inventory/Artifact.hpp"
#include "Utils/Random.hpp"

const std::vector<int> Artifact::sandsMainStats = {
        STAT_HP_PERCENT,
        STAT_ATK_PERCENT
};
const std::vector<int> Artifact::gobletMainStats = {
        STAT_HP_PERCENT,
        STAT_ATK_PERCENT,
        STAT_ELEMENTAL_DAMAGE
};
const std::vector<int> Artifact::circletMainStats = {
        STAT_HP_PERCENT,
        STAT_ATK_PERCENT,
        STAT_CRIT_RATE,
        STAT_CRIT_DAMAGE
};

std::unordered_map<int, double> Artifact::mainStatMaxValueMap = {
        {STAT_HP_FLAT,          4780},
        {STAT_HP_PERCENT,       0.466},
        {STAT_ATK_FLAT,         312},
        {STAT_ATK_PERCENT,      0.466},
        {STAT_CRIT_RATE,        0.311},
        {STAT_CRIT_DAMAGE,      0.622},
        {STAT_ELEMENTAL_DAMAGE, 0.466},
};

std::unordered_map<int, double> Artifact::statLvlUpMinValueMap = {
        {STAT_HP_FLAT,          210},
        {STAT_HP_PERCENT,       0.041},
        {STAT_ATK_FLAT,         16},
        {STAT_ATK_PERCENT,      0.041},
        {STAT_CRIT_RATE,        0.027},
        {STAT_CRIT_DAMAGE,      0.054},
        {STAT_ELEMENTAL_DAMAGE, 0.046},
};

std::unordered_map<int, const char *> Artifact::statNameMap = {
        {STAT_HP_FLAT,          "Flat Hp"},
        {STAT_HP_PERCENT,       "Hp"},
        {STAT_ATK_FLAT,         "Flat Attack"},
        {STAT_ATK_PERCENT,      "Attack"},
        {STAT_CRIT_RATE,        "Crit Rate"},
        {STAT_CRIT_DAMAGE,      "Crit Damage"},
        {STAT_ELEMENTAL_DAMAGE, "Elemental Damage"},
};

std::unordered_map<int, bool> Artifact::statIsFlatMap = {
        {STAT_HP_FLAT,          true},
        {STAT_HP_PERCENT,       false},
        {STAT_ATK_FLAT,         true},
        {STAT_ATK_PERCENT,      false},
        {STAT_CRIT_RATE,        false},
        {STAT_CRIT_DAMAGE,      false},
        {STAT_ELEMENTAL_DAMAGE, false},
};

Artifact::Artifact(int artfType_)
        : Equipment(ARTIFACT_MAX_LEVEL),
          remainingStats(), mainStat(),
          subStat1(), subStat2(), subStat3(), subStat4() {
    artfType = artfType_;
    for (int i = 0; i < STAT_N; i++) {
        remainingStats.push_back(i);
    }
    int rd;
    switch (artfType) {
        case ARTIFACT_FLOWER:
            mainStat.statType = STAT_HP_FLAT;
            break;
        case ARTIFACT_FEATHER:
            mainStat.statType = STAT_ATK_FLAT;
            break;
        case ARTIFACT_SANDS:
            rd = Random::getRandomIntEndExcl(0, (int) sandsMainStats.size());
            mainStat.statType = sandsMainStats[rd];
            break;
        case ARTIFACT_GLOBLET:
            rd = Random::getRandomIntEndExcl(0, (int) gobletMainStats.size());
            mainStat.statType = gobletMainStats[rd];
            break;
        case ARTIFACT_CIRCLET:
            rd = Random::getRandomIntEndExcl(0, (int) circletMainStats.size());
            mainStat.statType = circletMainStats[rd];
            break;
        default:
            break;
    }
    eraseStat(mainStat.statType);
    mainStat.statValue = getLevelMultiplier() * Artifact::mainStatMaxValueMap[mainStat.statType];
    subStat1.statType = getRandomStatAndErase();
    subStat2.statType = getRandomStatAndErase();
    subStat3.statType = getRandomStatAndErase();
    subStat4.statType = getRandomStatAndErase();
    rollSubStat(&subStat1);
    rollSubStat(&subStat2);
    rollSubStat(&subStat3);
    rollSubStat(&subStat4);
}

void Artifact::eraseStat(int statType) {
    remainingStats.erase(
            std::remove(remainingStats.begin(), remainingStats.end(), statType),
            remainingStats.end()
    );
}

int Artifact::getRandomStat() {
    int rdIndex = Random::getRandomIntEndExcl(0, (int) remainingStats.size());
    return remainingStats[rdIndex];
}

int Artifact::getRandomStatAndErase() {
    int rdStat = getRandomStat();
    eraseStat(rdStat);
    return rdStat;
}

double Artifact::getStatValue(int statType) const {
    double value = 0;
    if (mainStat.statType == statType) value += mainStat.statValue;
    if (subStat1.statType == statType) value += subStat1.statValue;
    if (subStat2.statType == statType) value += subStat2.statValue;
    if (subStat3.statType == statType) value += subStat3.statValue;
    if (subStat4.statType == statType) value += subStat4.statValue;
    return value;
}

StatInfo *Artifact::getStatInfo(int i) {
    switch (i) {
        case 0:
            return &mainStat;
        case 1:
            return &subStat1;
        case 2:
            return &subStat2;
        case 3:
            return &subStat3;
        case 4:
            return &subStat4;
        default:
            return nullptr;
    }
}

void Artifact::onLevelUp() {
    if (getLevel() % 4 == 0) {
        int rdSubStat = Random::getRandomInt(1, 4);
        switch (rdSubStat) {
            case 1:
                rollSubStat(&subStat1);
                break;
            case 2:
                rollSubStat(&subStat2);
                break;
            case 3:
                rollSubStat(&subStat3);
                break;
            case 4:
                rollSubStat(&subStat4);
                break;
            default:
                break;
        }
    }

    mainStat.statValue = getLevelMultiplier() * mainStatMaxValueMap[mainStat.statType];
}

void Artifact::sprintfStat(char *st, const char *fmt, int i) {
    StatInfo *artfStat = getStatInfo(i);
    sprintf(st, fmt, Artifact::getStatName(artfStat->statType),
            Artifact::isStatFlatName(artfStat->statType) ? artfStat->statValue : artfStat->statValue * 100.,
            Artifact::isStatFlatName(artfStat->statType) ? '\0' : '%');
}

void Artifact::rollSubStat(StatInfo *subStat) {
    double minRoll = statLvlUpMinValueMap[subStat->statType];
    double maxRoll = minRoll * ROLL_MAX_VALUE_PERCENT;
    subStat->statValue += Random::getRandomReal(minRoll, maxRoll);
}

void Artifact::getArtifactInfo(int i, std::string *imgPath, std::string *name) {
    if (imgPath != nullptr) *imgPath = "res/gfx/inventory/";
    switch (i) {
        case 0:
            *name = "Flower";
            break;
        case 1:
            *name = "Feather";
            break;
        case 2:
            *name = "Sand";
            break;
        case 3:
            *name = "Goblet";
            break;
        case 4:
            *name = "Circlet";
            break;
        default:
            *name = "";
            break;
    }
    if (imgPath != nullptr) *imgPath += "A" + *name + ".png";
}

const char *Artifact::getStatName(int statType) {
    return statNameMap[statType];
}

bool Artifact::isStatFlatName(int statType) {
    return statIsFlatMap[statType];
}
