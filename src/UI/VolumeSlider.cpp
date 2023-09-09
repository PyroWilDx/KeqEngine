//
// Created by pyrowildx on 12/08/2023.
//

#include <cmath>
#include "UI/VolumeSlider.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Utils.hpp"
#include "Entity/Text.hpp"

VolumeSlider::VolumeSlider(double x, double y, int renderW,
                           SDL_Color *filledColor, int currVolume)
        : Button(x, y, renderW, renderW / 10, 0),
          filledColor() {
    this->swapColorOnClick = false;
    this->changeColor(COLOR_WHITE);
    SDL_Color blackColor = {COLOR_BLACK_FULL};
    char volTxt[4];
    sprintf(volTxt, "%d\n", currVolume);
    this->addText(volTxt, &blackColor,
                  "res/fonts/JetBrainsMono-Regular.ttf", 16);

    this->filledColor = *filledColor;
    this->pickerX = getPickerXFromVolume(currVolume);
}

int VolumeSlider::getPickerXFromVolume(int volume) {
    return (int) (WorldEntity::getX() + round((double) getRenderW() * ((double) volume / 100.)));
}

int VolumeSlider::getCurrentVolume() {
    auto filledW = (double) (pickerX - WorldEntity::getX());
    int currVolume = (int) round((filledW * 100.) / (double) getRenderW());
    currVolume = std::max(currVolume, 0);
    currVolume = std::min(currVolume, 100);
    return currVolume;
}

void VolumeSlider::setPickerXFromNewX(int newX) {
    pickerX = newX;
    pickerX = getPickerXFromVolume(getCurrentVolume());
}

void VolumeSlider::onClick(int mouseX, int mouseY) {
    setPickerXFromNewX(mouseX);

    Button::onClick(mouseX, mouseY);
}

void VolumeSlider::onClickedMove(int mouseX, int mouseY, bool isMouseOnButton) {
    if (isMouseOnButton) setPickerXFromNewX(mouseX);

    Button::onClickedMove(mouseX, mouseY, isMouseOnButton);
}

void VolumeSlider::renderSelf(SDL_Renderer *gRenderer) {
    char volTxt[4];
    sprintf(volTxt, "%d", getCurrentVolume());
    changeText(volTxt);

    Button::renderSelf(gRenderer);

    SDL_Rect filledRect = {WorldEntity::getX(), WorldEntity::getY(),
                           pickerX - WorldEntity::getX(), getRenderH()};
    WindowRenderer::renderRect(&filledRect, true,
                               filledColor.r, filledColor.g,
                               filledColor.b, filledColor.a,
                               gRenderer,
                               false);

    buttonText->renderSelf(gRenderer);
}
