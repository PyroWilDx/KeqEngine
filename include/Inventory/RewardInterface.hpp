//
// Created by pyrow on 09/06/2024.
//

#ifndef REWARDINTERFACE_HPP
#define REWARDINTERFACE_HPP

class RewardInterface {

public:
    RewardInterface() = default;

    virtual ~RewardInterface() = default;

    virtual void addToKq() = 0;

};

#endif
