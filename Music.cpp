#include "Music.h"

DemoSystem::Music::Music() {
    
}

DemoSystem::Music::~Music() {
    BASS_Free();
}

bool DemoSystem::Music::Initialize(int frequency, std::string file) {
    if(!BASS_Init(-1, frequency, BASS_DEVICE_STEREO, 0, NULL)) {
        return false;
    }
    if(!BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, BASS_STREAM_PRESCAN)) {
        return false;
    }
}