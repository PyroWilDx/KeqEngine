//
// Created by pyrow on 03/08/2023.
//

#include <string>
#include "StructForEntity/SoundSheet.hpp"
#include "Utils/Random.hpp"
#include "Utils/Sound.hpp"
#include "Utils/Utils.hpp"

SoundSheet::SoundSheet(int soundSheetLength) {
    this->soundSheetLength = soundSheetLength;
    this->startPath = "";
    soundSheet = new std::vector<const char *>[soundSheetLength];
}

SoundSheet::~SoundSheet() {
    delete[] soundSheet;
}

void SoundSheet::copySoundPaths(int dstCode, int srcCode) {
    for (const char *path: soundSheet[srcCode]) {
        soundSheet[dstCode].push_back(path);
    }
}

void SoundSheet::copySoundPathsStartString(int dstCode, int srcCode,
                                           const std::string &startStrPath) {
    for (const char *path: soundSheet[srcCode]) {
        std::string pathStr = std::string(path);
        if (pathStr.compare(0, startStrPath.size(), startStrPath) == 0) {
            soundSheet[dstCode].push_back(path);
        }
    }
}

void SoundSheet::playSoundAtIndex(int i, int soundCode) {
    myAssert(soundCode >= 0 && soundCode < soundSheetLength, "Sound Code Out of Bounds !");

    std::vector<const char *> soundVector = soundSheet[soundCode];
    const char *soundPath = soundVector[i];
    std::string fullPathStr = std::string(soundPath);
    if (!startPath.empty()) {
        fullPathStr = startPath + "/" + fullPathStr;
    }

    Sound::playAudioChunk(fullPathStr.c_str());
}

void SoundSheet::playRandomSoundIndexed(int startIndex, int endIndex,
                                        int soundCode) {
    std::vector<const char *> soundVector = soundSheet[soundCode];
    int size = (int) soundVector.size();
    myAssert(size > 0, "Sound Vector is Empty !");

    if (startIndex == -1) startIndex = size - 1;
    if (endIndex == -1) endIndex = size - 1;

    int rdInt = Random::getRandomInt(startIndex, endIndex);
    playSoundAtIndex(rdInt, soundCode);
}

void SoundSheet::playRandomSoundIndexedOrNone(int startIndex, int endIndex,
                                              double probNone, int soundCode) {
    double rdReal = Random::getRandomReal();
    if (rdReal > probNone) {
        playRandomSoundIndexed(startIndex, endIndex, soundCode);
    }
}

void SoundSheet::playRandomSound(int soundCode) {
    playRandomSoundIndexed(0, -1, soundCode);
}

void SoundSheet::playRandomSoundOrNone(double probNone, int soundCode) {
    playRandomSoundIndexedOrNone(0, -1, probNone, soundCode);
}

void SoundSheet::playRandomSoundStartString(const std::string &startStr, int soundCode) {
    // Supposing that sound paths of the same start follow each other
    std::vector<const char *> soundVector = soundSheet[soundCode];

    int startIndex = -1;
    int endIndex = -1;
    int i = 0;
    for (const char *soundPath: soundVector) {
        std::string soundName = std::string(soundPath);
        if (startPath.empty()) {
            size_t lastSlashIndex = startPath.find_last_of('/');
            soundName = soundName.substr(lastSlashIndex + 1);
        }
        if (soundName.compare(0, startStr.size(), startStr) == 0) {
            if (startIndex == -1) {
                startIndex = i;
                endIndex = i;
            } else {
                endIndex++;
            }
        }
        i++;
    }
    playRandomSoundIndexed(startIndex, endIndex, soundCode);
}
