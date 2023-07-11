//
// Created by pyrowildx on 07/07/2023.
//

#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "EntityBase/WorldEntity.hpp"
#include "Entity/Text.hpp"

typedef enum {
    BUTTON_IDLE = 0,
    BUTTON_CLICKED
} ButtonState;

class Button : public WorldEntity {

public:
    Button(double x, double y, int renderW, int renderH);

    void changeColor(Uint8 r, Uint8 g, Uint8 b);

    void addText(const char *text, const SDL_Color *color,
                 const char *fontPath, int fontSize);

    inline bool shouldTranslate() override { return false; }

    void renderSelf(SDL_Renderer *gRenderer) override;

    void onClick();

    void onClickRelease(bool isMouseOnButton);

    inline void setCallBack(void (*fCallBack_)(void *onClickParams)) { fCallBack = fCallBack_; }

    inline void setOnClickParams(void *onClickParams_) { onClickParams = onClickParams_; }

    inline void setState(ButtonState buttonState_) { buttonState = buttonState_; }

private:
    void (*fCallBack)(void *onClickParams);

    void *onClickParams;
    ButtonState buttonState;
    SDL_Color buttonColor;
    Text *buttonText;

};

#endif
