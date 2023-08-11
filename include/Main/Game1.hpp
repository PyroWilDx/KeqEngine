//
// Created by pyrowildx on 09/07/2023.
//

#ifndef GAME1_HPP
#define GAME1_HPP

#include "MainInterface.hpp"

class Game1 : public MainInterface<Game1> {

public:
    Game1() = delete;

    static void RunImpl();

private:

};

#endif
