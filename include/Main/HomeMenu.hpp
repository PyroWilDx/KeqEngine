//
// Created by pyrowildx on 09/07/2023.
//

#ifndef HOMEMENU_HPP
#define HOMEMENU_HPP

#include "MainInterface.hpp"

class MainHomeMenu : public MainInterface<MainHomeMenu> {

public:
    MainHomeMenu() = delete;

    static void RunImpl();

private:

};

#endif
