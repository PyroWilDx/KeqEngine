//
// Created by pyrow on 05/01/2024.
//

#ifndef EXPLOSLIME_HPP
#define EXPLOSLIME_HPP

#include <vector>
#include "MainInterface.hpp"

class World;
class Block;

typedef World *(*LvlFuncPointer)(Block **);

class ExploSlime : public MainInterface<ExploSlime> {

public:
    ExploSlime() = delete;

    static void RunImpl();

    static World *Level0(Block **goalBlock);

    static World *Level1(Block **goalBlock);

    static World *Level2(Block **goalBlock);

    static void addWinMenu(World *gWorld, bool *gRunning, int winTime);

    static int iLevel;

    static std::vector<LvlFuncPointer> lvlFuncs;

private:

};


#endif
