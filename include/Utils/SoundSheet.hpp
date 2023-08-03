//
// Created by pyrow on 03/08/2023.
//

#ifndef SOUNDSHEET_HPP
#define SOUNDSHEET_HPP

#include <vector>

class SoundSheet {

public:
    explicit SoundSheet(int soundSheetLength);

    ~SoundSheet();

    inline void initSoundSheetElement(int soundCode) {};

    template<typename T, typename... Args>
    inline void initSoundSheetElement(int soundCode, const T val, const Args... args) {
        soundSheet[soundCode].push_back(val);
        initSoundSheetElement(soundCode, args...);
    };

    void playRandomSoundFromCode(int soundCode);

private:
    int soundSheetLength;
    std::vector<const char *> *soundSheet;

};

#endif
