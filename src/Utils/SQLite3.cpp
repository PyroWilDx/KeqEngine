//
// Created by pyrow on 06/01/2024.
//

#include <SDL2/SDL.h>
#include <sqlite3.h>
#include "Utils/SQLite3.hpp"
#include "Utils/Global.hpp"

bool execSQL(const char *rq, int (*callBack)(void *, int, char **, char **), void *data) {
    int result = sqlite3_exec(Global::db, rq, callBack, data, nullptr);
    bool succ = result == SQLITE_OK;
    if (!succ) {
        SDL_Log("SQLite Error : %s", sqlite3_errmsg(Global::db));
    }
    return succ;
}
