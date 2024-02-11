//
// Created by pyrowildx on 11/07/2023.
//

#ifndef EVENTS_HPP
#define EVENTS_HPP

#include <SDL2/SDL_events.h>

enum {
    KEY_UNDEFINED = -1,
    KEY_MOUSE_LEFT,
    KEY_MOUSE_RIGHT,
    KEY_Z,
    KEY_Q,
    KEY_S,
    KEY_D,
    KEY_A,
    KEY_E,
    KEY_R,
    KEY_SPACE,
    KEY_SHIFT,
    KEY_ENUM_N
};

typedef struct gStateInfo {
    bool gRunning;
    bool gPaused;
    bool runFrame;
    int lastDt;
    bool debugMode;
} gStateInfo;

#define DEFAULT_GAME_STATE_INFO {true, false, false, 0, false}

class Events {

public:
    Events() = delete;

    static void initEvents();

    static void cleanUp();

    static void onWindowResize(int newW, int newH);

    static void onMouse(SDL_Event *event);

    static void onQuit(gStateInfo *gInfo);
    
    static void handleBasicEvents(SDL_Event *event, int *pKey, gStateInfo *gInfo);

    static void callMainFunc(bool *gRunningLastMain, void (*gMain)());

private:

};

#endif
