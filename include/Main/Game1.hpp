//
// Created by pyrowildx on 09/07/2023.
//

#ifndef GAME1_HPP
#define GAME1_HPP

#include "MainInterface.hpp"

class MainGame1 : public MainInterface<MainGame1> {

public:
    MainGame1() = delete;

    static void RunImpl();

private:

};

#endif
