//
// Created by pyrow on 01/04/2024.
//

#ifndef TOASTTEXT_HPP
#define TOASTTEXT_HPP

#include "AbstractEntity/WorldEntity.hpp"

class Text;

class ToastText : public WorldEntity {

public:
    ToastText(int renderW, const char *cStr, int showDuration_);

    ~ToastText() override;

    bool shouldTranslate() override;

    bool onGameFrame() override;

    void renderSelf(SDL_Renderer *gRenderer) override;

private:
    Text *toastText;
    int showDuration;

};

#endif
