//
// Created by pyrow on 06/01/2024.
//

#ifndef TIMER_HPP
#define TIMER_HPP

#include "Text.hpp"

class Timer : public Text {

public:
    Timer(double x, double y, SDL_Color *timerTextColor,
          int timerFontSize, SDL_Color *outlColor, int outlSize);

    Timer(double x, double y, SDL_Color *timerTextColor, int timerFontSize);

    ~Timer() override = default;

    void start();

    void stop();

    void resume();

    bool onGameFrame() override;

    [[nodiscard]] inline bool hasStarted() const { return started; }

    [[nodiscard]] inline int getElapsedTime() const { return elapsedTime; }

private:
    bool started;
    bool stopped;
    int elapsedTime;

};


#endif
