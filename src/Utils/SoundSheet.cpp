//
// Created by pyrow on 03/08/2023.
//

#include "Utils/SoundSheet.hpp"
#include "Utils/Random.hpp"
#include "Utils/Sound.hpp"
#include "Utils/Utils.hpp"

SoundSheet::SoundSheet(int soundSheetLength) {
    this->soundSheetLength = soundSheetLength;
    soundSheet = new std::vector<const char *>[soundSheetLength];
}

SoundSheet::~SoundSheet() {
    delete[] soundSheet;
}

void SoundSheet::playRandomSoundFromCode(int soundCode) {
    std::vector<const char *> soundVector = soundSheet[soundCode];
    int size = (int) soundVector.size();
    myAssert(size > 0, "Sound Code Out of Bounds !\n");
    int rdInt = Random::getRandomIntEndExcl(0, (int) soundVector.size());
    const char *soundPath = soundVector[rdInt];
    playAudioChunk(soundPath);
}
