//
// Created by pyrow on 09/06/2024.
//

#include "Inventory/Money.hpp"
#include "Keqing.hpp"

Money::Money(int moneyCount) : RewardInterface() {
    value = moneyCount;
}

void Money::addToKq() {
    Keqing::getInstance()->addMoneyCount(value);
}
