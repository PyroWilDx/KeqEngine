//
// Created by pyrowildx on 31/07/2023.
//

#include <cstdio>
#include "Text/FPSText.hpp"
#include "Utils/Global.hpp"
#include "Utils/Colors.hpp"

FPSText::FPSText()
        : Text(10, 0, "FPS : 0", &Colors::dColorWhite,
               38, &Colors::dColorBlack, 2,
               false) {
    accumulatedFPSTime = 0;
    accumulatedFrames = 0;
}

bool FPSText::onGameFrame() {
    bool doNext = Text::onGameFrame();

    if (doNext) {
        if (accumulatedFPSTime > 1000) {
            char text[16];
            sprintf(text, "FPS : %d", accumulatedFrames);
            changeText(text);
            accumulatedFPSTime = 0;
            accumulatedFrames = 0;
        }
        accumulatedFPSTime += Global::dt;
        accumulatedFrames++;
    }

    return doNext;
}
