//
// Created by pyrow on 05/01/2024.
//

#ifndef GOALGAME_HPP
#define GOALGAME_HPP

#include <vector>
#include "MainInterface.hpp"

class World;

typedef World *(*LvlFuncPointer)();

class GoalGame : public MainInterface<GoalGame> {

public:
    GoalGame() = delete;

    static void RunImpl();

    static World * Level1();

    static World * Level2();

    static int iLevel;

    static std::vector<LvlFuncPointer> lvlFuncs;

private:

};


#endif
