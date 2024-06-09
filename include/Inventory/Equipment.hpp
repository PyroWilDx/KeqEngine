//
// Created by pyrow on 01/04/2024.
//

#ifndef EQUIPMENT_HPP
#define EQUIPMENT_HPP

class Equipment {

public:
    explicit Equipment(int maxLevel_);

    virtual ~Equipment() = default;

    bool levelUp();

    virtual void onLevelUp() = 0;

    [[nodiscard]] double getLevelMultiplier() const;

    void setLevel(int newLevel);

    [[nodiscard]] inline int getMaxLevel() const { return maxLevel; }

    [[nodiscard]] inline int getLevel() const { return eLevel; }

private:
    int maxLevel;
    int eLevel;

};

#endif
