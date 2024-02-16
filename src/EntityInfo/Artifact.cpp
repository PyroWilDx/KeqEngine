//
// Created by pyrow on 11/02/2024.
//

#include <algorithm>
#include "EntityInfo/Artifact.hpp"
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
        {STAT_HP_PERCENT,       4.1},
        {STAT_ATK_FLAT,         16},
        {STAT_ATK_PERCENT,      4.1},
        {STAT_CRIT_RATE,        2.7},
        {STAT_CRIT_DAMAGE,      5.4},
        {STAT_ELEMENTAL_DAMAGE, 4.6},
};

Artifact::Artifact(int artfType)
        : artfLevel(1), remainingStats(), mainStat(),
          subStat1(), subStat2(), subStat3(), subStat4() {
    for (int i = 0; i < STAT_N; i++) {
        remainingStats[i] = i;
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
    mainStat.statValue = getLevelCoeff() * Artifact::mainStatMaxValueMap[mainStat.statType];
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

void Artifact::rollSubStat(StatInfo *subStat) {
    double minRoll = statLvlUpMinValueMap[subStat->statType];
    double maxRoll = minRoll * ROLL_MAX_VALUE_PERCENT;
    subStat->statValue += Random::getRandomReal(minRoll, maxRoll);
}

double Artifact::getLevelCoeff() const {
    return (double) artfLevel / ARTIFACT_MAX_LEVEL;
}

bool Artifact::levelUp() {
    if (artfLevel >= ARTIFACT_MAX_LEVEL) return false;

    artfLevel++;

    if (artfLevel % 4 == 0) {
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
                return false;
        }
    }

    mainStat.statValue = getLevelCoeff() * mainStatMaxValueMap[mainStat.statType];

    return true;
}
