//
// Created by pyrow on 06/01/2024.
//

#include "Text/Timer.hpp"
#include "Utils/Global.hpp"

Timer::Timer(double x, double y, SDL_Color *timerTextColor,
             int timerFontSize, SDL_Color *outlColor, int outlSize)
        : Text(x, y, "0", timerTextColor,
               timerFontSize, outlColor, outlSize,
               false) {
    this->started = false;
    this->stopped = false;
    this->elapsedTime = 0;
}

Timer::Timer(double x, double y, SDL_Color *timerTextColor,
             int timerFontSize)
        : Timer(x, y, timerTextColor,
                timerFontSize, nullptr, 0) {

}

void Timer::start() {
    started = true;
    stopped = false;
}

void Timer::stop() {
    stopped = true;
}

void Timer::resume() {
    stopped = false;
}

bool Timer::onGameFrame() {
    bool doNext = Text::onGameFrame();

    if (doNext) {
        if (started && !stopped) {
            elapsedTime += Global::dt;
            char text[16];
            sprintf(text, "%.2f", elapsedTime * 0.001);
            changeText(text);
        }
    }

    return doNext;
}
