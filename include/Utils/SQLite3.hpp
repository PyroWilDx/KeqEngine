//
// Created by pyrow on 06/01/2024.
//

#ifndef SQLITE3_HPP
#define SQLITE3_HPP

bool execSQL(const char *rq, int (*callBack)(void *, int, char **, char **), void *data);

#endif
