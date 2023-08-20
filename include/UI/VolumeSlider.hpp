//
// Created by pyrowildx on 12/08/2023.
//

#ifndef VOLUMESLIDER_HPP
#define VOLUMESLIDER_HPP

#include "UI/Button.hpp"

class VolumeSlider : public Button {

public:
    VolumeSlider(double x, double y, int renderW,
                 SDL_Color *filledColor, int currVolume);

    [[nodiscard]] int getPickerXFromVolume(int volume);

    [[nodiscard]] int getCurrentVolume();

    void setPickerXFromNewX(int newX);

    void onClick(int mouseX, int mouseY) override;

    void onClickedMove(int mouseX, int mouseY, bool isMouseOnButton) override;

    void renderSelf(SDL_Renderer *gRenderer) override;

private:
    int pickerX;
    SDL_Color filledColor;

};

#endif
