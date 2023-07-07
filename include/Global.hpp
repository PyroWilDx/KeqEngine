//
// Created by pyrow on 25/06/2023.
//

#ifndef GLOBAL_H
#define GLOBAL_H

#include "World.hpp"

class Global {

public:
    Global() = delete;

    static void initGlobal();

    static int windowWidth;
    static int windowHeight;

    static bool pressedKeys[KEY_ENUM_N];
    static int pressedTime[KEY_ENUM_N];
    static int lastPressedTime[KEY_ENUM_N];

    static int currentTime;
    static int dt;

    static World *currentWorld;

private:

};

#endif
