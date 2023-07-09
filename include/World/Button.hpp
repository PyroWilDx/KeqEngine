//
// Created by pyrow on 07/07/2023.
//

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "EntityBase/WorldEntity.hpp"
#include "Entity/Text.hpp"

class Button : public WorldEntity {

public:
    Button(double x, double y, int renderW, int renderH);

    void changeColor(Uint8 r, Uint8 g, Uint8 b);

    void addImage(const char *imgPath, int frameW = 0, int frameH = 0);

    void addText(const char *text, const SDL_Color *color,
                 const char *fontPath, int fontSize);

    inline bool shouldTranslate() override { return false; }

    void renderSelf(SDL_Renderer *gRenderer) override;

    void onClick();

    inline void setCallBack(void (*fCallBack_)(void *params)) { fCallBack = fCallBack_; }

private:
    void (*fCallBack)(void *params);

    SDL_Color buttonColor;
    Text *buttonText;

};

#endif
