//
// Created by pyrow on 31/07/2023.
//

#ifndef FPSTEXT_HPP
#define FPSTEXT_HPP

#include "Entity/Text.hpp"

class FPSText : public Text {

    using Text::Text;

public:
    ~FPSText() override = default;

    void onGameFrame() override;

private:
    int accumulatedFPSTime;
    int accumulatedFrames;

};

#endif
