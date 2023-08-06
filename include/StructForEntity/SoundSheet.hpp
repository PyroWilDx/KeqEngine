//
// Created by pyrow on 03/08/2023.
//

#ifndef SOUNDSHEET_HPP
#define SOUNDSHEET_HPP

#include <string>
#include <vector>

class SoundSheet {

public:
    explicit SoundSheet(int soundSheetLength);

    ~SoundSheet();

    void pushSoundPaths(int soundCode) {};

    template<typename T, typename... Args>
    void pushSoundPaths(int soundCode, const T val, const Args... args) {
        soundSheet[soundCode].push_back(val);
        pushSoundPaths(soundCode, args...);
    };

    void copySoundPaths(int dstCode, int srcCode);

    void copySoundPathsStartString(int dstCode, int srcCode,
                                   const std::string &startStrPath);

    void playSoundAtIndex(int i, int soundCode);

    void playRandomSoundIndexed(int startIndex, int endIndex,
                                int soundCode);

    void playRandomSoundIndexedOrNone(int startIndex, int endIndex,
                                      double probNone, int soundCode);

    void playRandomSound(int soundCode);

    void playRandomSoundOrNone(double probNone, int soundCode);

    void playRandomSoundStartString(const std::string &startStr, int soundCode);

    inline void setSoundStartPath(std::string &soundStartPath) { startPath = soundStartPath; }

private:
    int soundSheetLength;
    std::string startPath;
    std::vector<const char *> *soundSheet;

};

#endif
