//
// Created by pyrowildx on 03/08/2023.
//

#include "Utils/Random.hpp"

std::random_device Random::rd;
std::mt19937 Random::generator = std::mt19937(rd());
std::uniform_int_distribution<int> Random::randomInt;
std::uniform_real_distribution<double> Random::randomReal;

void Random::initRandom() {

}

void Random::cleanUp() {

}

int Random::getRandomInt(int start, int end) {
    randomInt = std::uniform_int_distribution<int>(start, end);
    return randomInt(generator);
}

int Random::getRandomIntEndExcl(int start, int end) {
    return Random::getRandomInt(start, end - 1);
}

double Random::getRandomReal() {
    randomReal = std::uniform_real_distribution<double>(0., 1.);
    return randomReal(generator);
}

double Random::getRandomReal(double start, double end) {
    return (Random::getRandomReal() * (end - start) + start);
}
