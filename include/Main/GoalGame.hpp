//
// Created by pyrow on 05/01/2024.
//

#ifndef GOALGAME_HPP
#define GOALGAME_HPP

#include <vector>
#include "MainInterface.hpp"

class World;
class Block;

typedef World *(*LvlFuncPointer)(Block **);

class GoalGame : public MainInterface<GoalGame> {

public:
    GoalGame() = delete;

    static void RunImpl();

    static World * Level0(Block **goalBlock);

    static World * Level1(Block **goalBlock);

    static int iLevel;

    static std::vector<LvlFuncPointer> lvlFuncs;

private:

};


#endif
