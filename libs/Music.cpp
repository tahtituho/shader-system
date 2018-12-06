#include "Music.h"

DemoSystem::Music::Music() {
    
}

DemoSystem::Music::~Music() {
    BASS_SampleFree(this->stream);
    BASS_Free();
}

bool DemoSystem::Music::initialize(int frequency, std::string file) {
    if(!BASS_Init(-1, frequency, BASS_DEVICE_STEREO, 0, NULL)) {
        return false;
    }
    this->stream = BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, BASS_STREAM_PRESCAN);
    if(this->stream == 0) {
        int error = BASS_ErrorGetCode();
        return false;
    }
   
    return true;

}

void DemoSystem::Music::play() {
    !BASS_ChannelPlay(this->stream, false);
}

int DemoSystem::Music::position() {
    //Not yet implemented
    return 0;
}

void DemoSystem::Music::seek(int position) {
    //Not yet implemented
}