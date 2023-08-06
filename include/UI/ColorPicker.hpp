//
// Created by pyrow on 13/07/2023.
//

#ifndef COLORPICKER_HPP
#define COLORPICKER_HPP

#include "Button.hpp"

class ColorPicker : public Button {

public:
    ColorPicker(double x, double y, int renderW, int renderH, Uint32 currRGBA = 0);

    ~ColorPicker() override;

//    SDL_Rect getRenderRect() override;

    void fillPixels(Uint32 currRGBA);

    [[nodiscard]] Uint32 getCurrentRGBA() const;

    void onClick(int mouseX, int mouseY) override;

    int getPickerSize();

    void renderSelf(SDL_Renderer *gRenderer) override;

private:
    Uint32 **absPixels;
    int pickerX, pickerY;

};

#endif
