//
// Created by pyrowildx on 09/07/2023.
//

#ifndef HOMEMENU_HPP
#define HOMEMENU_HPP

#include "MainInterface.hpp"

class HomeMenu : public MainInterface<HomeMenu> {

public:
    HomeMenu() = delete;

    static void RunImpl();

private:

};

#endif
