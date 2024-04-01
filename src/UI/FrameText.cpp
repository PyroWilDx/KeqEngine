//
// Created by pyrow on 31/03/2024.
//

#include "UI/FrameText.hpp"
#include "Text/Text.hpp"
#include "WindowRenderer.hpp"
#include "Utils/Colors.hpp"

FrameText::FrameText(double x, double y, int renderW, int renderH,
                     SDL_Color *outlineColor_, SDL_Color *bgColor_)
        : WorldEntity(x, y, renderW, renderH, WORLD_FRAME_TEXT) {
    outlineColor = outlineColor_;
    bgColor = bgColor_;
    textVector = std::vector<Text *>();
}

FrameText::FrameText(double x, double y, int renderW, int renderH)
        : FrameText(x, y, renderW, renderH,
                    &Colors::dColorWhite, &Colors::dColorDarkGray) {

}

FrameText::~FrameText() {
    removeTexts();
}

void FrameText::addTexts(std::initializer_list<Text *> texts) {
    textVector.insert(textVector.end(), texts.begin(), texts.end());
}

void FrameText::removeTexts() {
    for (Text *text : textVector) {
        delete text;
    }
    textVector.clear();
}

void FrameText::changeTexts(std::initializer_list<Text *> texts) {
    removeTexts();
    addTexts(texts);
}

void FrameText::renderSelf(SDL_Renderer *gRenderer) {
    SDL_Rect outlineRect = getRenderRect();
    SDL_Rect bgRect = outlineRect;

    outlineRect.x -= 6;
    outlineRect.y -= 6;
    outlineRect.w += 12;
    outlineRect.h += 12;

    WindowRenderer::renderRect(&outlineRect, true,
                               outlineColor, gRenderer,
                               true, false);
    WindowRenderer::renderRect(&bgRect, true,
                               bgColor, gRenderer,
                               true, false);

    Text *lastText = nullptr;
    for (Text *text: textVector) {
        if (lastText != nullptr)
            text->moveTo(lastText->getX(),
                         lastText->getY() + lastText->getFontSize() + 6);
        else text->moveTo(WorldEntity::getX() + 6, WorldEntity::getY() + 6);
        text->renderSelf(gRenderer);
        lastText = text;
    }
}
