//
// Created by pyrowildx on 03/08/2023.
//

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include <random>

class Random {

public:
    Random() = delete;

    static void initRandom();

    static void cleanUp();

    static int getRandomInt(int start, int end);

    static int getRandomIntEndExcl(int start, int end);

    static double getRandomReal();

    static double getRandomReal(double start, double end);

private:
    static std::random_device rd;
    static std::mt19937 generator;
    static std::uniform_int_distribution<int> randomInt;
    static std::uniform_real_distribution<double> randomReal;

};

#endif
