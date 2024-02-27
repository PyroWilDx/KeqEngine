//
// Created by pyrow on 26/02/2024.
//

#ifndef SHOWTEST_HPP
#define SHOWTEST_HPP

#include "MainInterface.hpp"

class ShowTest : public MainInterface<ShowTest> {

public:
    ShowTest() = delete;

    static void RunImpl();

    static void RenderingShow();

    static void GameUIShow();

private:

};

#endif
