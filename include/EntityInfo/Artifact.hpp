//
// Created by pyrow on 11/02/2024.
//

#ifndef ARTIFACT_HPP
#define ARTIFACT_HPP

#include <string>
#include <vector>
#include <unordered_map>
#include "EntityInfo/Equipment.hpp"

#define ARTIFACT_MAX_LEVEL 20

#define ROLL_MAX_VALUE_PERCENT 1.42

enum {
    ARTIFACT_FLOWER = 0,
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
    STAT_ELEMENTAL_DAMAGE,
    STAT_N
};

typedef struct StatInfo {
    int statType;
    double statValue;
} StatInfo;

class Artifact : public Equipment {

public:
    explicit Artifact(int artfType_);

    void eraseStat(int statType);

    int getRandomStat();

    int getRandomStatAndErase();

    void onLevelUp() override;

    double getStatValue(int statType) const;

    [[nodiscard]] StatInfo *getStatInfo(int i);

    void sprintfStat(char *st, const char *fmt, int i);

    [[nodiscard]] inline int getArtfType() const { return artfType; }

    static void rollSubStat(StatInfo *subStat);

    static void getArtifactInfo(int i, std::string *imgPath, std::string *name);

    static const char *getStatName(int statType);

    static bool isStatFlatName(int statType);

    static const std::vector<int> sandsMainStats;
    static const std::vector<int> gobletMainStats;
    static const std::vector<int> circletMainStats;

    static std::unordered_map<int, double> mainStatMaxValueMap;

    static std::unordered_map<int, double> statLvlUpMinValueMap;

    static std::unordered_map<int, const char *> statNameMap;
    static std::unordered_map<int, bool> statIsFlatMap;

private:
    int artfType;
    std::vector<int> remainingStats;
    StatInfo mainStat;
    StatInfo subStat1;
    StatInfo subStat2;
    StatInfo subStat3;
    StatInfo subStat4;

};

#endif
