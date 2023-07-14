//
// Created by pyrow on 13/07/2023.
//

#ifndef COLORPICKER_HPP
#define COLORPICKER_HPP

#include "Button.hpp"

class ColorPicker : public Button {

public:
    ColorPicker(double x, double y, int renderW, int renderH);

    void onClick(int mouseX, int mouseY) override;

    int getPickerSize();

    void renderSelf(SDL_Renderer *gRenderer) override;

    inline Uint32 getCurrentRGBA() { return currentRGBA; }

private:
    int pickerX, pickerY;
    Uint32 currentRGBA;

};

#endif
