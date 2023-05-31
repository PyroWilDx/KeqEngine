//
// Created by pyrowildx on 13/05/23.
//

#ifndef UTILS_HPP
#define UTILS_HPP

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

const float Z_MULTIPLIER = 0.6f;
const int MIN_Z = -102;
const int MAX_Z = 54;

void myAssert(bool expr, const char *msg, const char *err);

#endif
