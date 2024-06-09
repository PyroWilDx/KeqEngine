//
// Created by pyrow on 09/06/2024.
//

#ifndef MONEY_HPP
#define MONEY_HPP

#include "RewardInterface.hpp"

class Money : public RewardInterface {

public:
    explicit Money(int moneyCount);

    void addToKq() override;

private:
    int value;

};

#endif
