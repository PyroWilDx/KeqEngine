//
// Created by pyrow on 05/01/2024.
//

#ifndef GOALGAME_HPP
#define GOALGAME_HPP

#include <vector>
#include "MainInterface.hpp"

typedef void (*LvlFuncPointer)();

class GoalGame : public MainInterface<GoalGame> {

public:
    GoalGame() = delete;

    static void RunImpl();

    static void Level1();

    static void Level2();

    static int iLevel;

    static std::vector<LvlFuncPointer> lvlFuncs;

private:

};


#endif
