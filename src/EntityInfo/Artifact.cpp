//
// Created by pyrow on 11/02/2024.
//

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
        : artfLevel(1), mainStat(),
          subStat1(), subStat2(), subStat3(), subStat4() {
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
    mainStat.statValue = getLevelCoeff() * Artifact::mainStatMaxValueMap[mainStat.statType];
}

double Artifact::getLevelCoeff() const {
    return (double) artfLevel / ARTIFACT_MAX_LEVEL;
}
