//
// Created by pyrow on 31/03/2024.
//

#ifndef FRAMETEXT_HPP
#define FRAMETEXT_HPP

#include <vector>
#include "AbstractEntity/WorldEntity.hpp"

class Text;

class FrameText : public WorldEntity {

public:
    FrameText(double x, double y, int renderW, int renderH,
              SDL_Color *outlineColor_, SDL_Color *bgColor_);

    FrameText(double x, double y, int renderW, int renderH);

    ~FrameText() override;

    void addTexts(std::initializer_list<Text *> texts);

    void removeTexts();

    void changeTexts(std::initializer_list<Text *> texts);

    void renderSelf(SDL_Renderer *gRenderer) override;

private:
    SDL_Color *outlineColor;
    SDL_Color *bgColor;
    std::vector<Text *> textVector;

};

#endif
