//
// Created by pyrowildx on 09/07/2023.
//

#ifndef DEBUGGAME_HPP
#define DEBUGGAME_HPP

#include "MainInterface.hpp"

class DebugGame : public MainInterface<DebugGame> {

public:
    DebugGame() = delete;

    static void RunImpl();

private:

};

#endif
