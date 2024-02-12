//
// Created by pyrow on 11/02/2024.
//

#ifndef ARTIFACT_HPP
#define ARTIFACT_HPP

#include <vector>
#include <unordered_map>

#define ARTIFACT_MAX_LEVEL 20

#define ROLL_MAX_VALUE_PERCENT 1.42

enum {
    ARTIFACT_FLOWER,
    ARTIFACT_FEATHER,
    ARTIFACT_SANDS,
    ARTIFACT_GLOBLET,
    ARTIFACT_CIRCLET,
};

enum {
    STAT_HP_FLAT,
    STAT_HP_PERCENT,
    STAT_ATK_FLAT,
    STAT_ATK_PERCENT,
    STAT_CRIT_RATE,
    STAT_CRIT_DAMAGE,
    STAT_ELEMENTAL_DAMAGE
};

typedef struct StatInfo {
    int statType;
    double statValue;
} StatInfo;

class Artifact {

public:
    explicit Artifact(int artfType);

    double getLevelCoeff() const;

    static const std::vector<int> sandsMainStats;
    static const std::vector<int> gobletMainStats;
    static const std::vector<int> circletMainStats;

    static std::unordered_map<int, double> mainStatMaxValueMap;

    static std::unordered_map<int, double> statLvlUpMinValueMap;

private:
    int artfLevel;
    StatInfo mainStat;
    StatInfo subStat1;
    StatInfo subStat2;
    StatInfo subStat3;
    StatInfo subStat4;

};

#endif
