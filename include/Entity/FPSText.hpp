//
// Created by pyrowildx on 31/07/2023.
//

#ifndef FPSTEXT_HPP
#define FPSTEXT_HPP

#include "Entity/Text.hpp"

class FPSText : public Text {

public:
    FPSText();

    ~FPSText() override = default;

    void onGameFrame() override;

private:
    int accumulatedFPSTime;
    int accumulatedFrames;

};

#endif
