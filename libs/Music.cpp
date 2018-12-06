#include "Music.h"

DemoSystem::Music::Music() {
    this->playing = false;
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
    BASS_ChannelPlay(this->stream, false);
    this->playing = true;
}

void DemoSystem::Music::pause() {
    BASS_ChannelPause(this->stream);
    this->playing = false;
}

bool DemoSystem::Music::isPlaying() {
    return this->playing;
}

double DemoSystem::Music::position() {
    QWORD bytePosition = BASS_ChannelGetPosition(this->stream, BASS_POS_BYTE);
    return BASS_ChannelBytes2Seconds(this->stream, bytePosition);
}

void DemoSystem::Music::seek(double position) {
    QWORD bytePosition = BASS_ChannelSeconds2Bytes(this->stream, position);
    BASS_ChannelSetPosition(this->stream, bytePosition, 
BASS_POS_BYTE);
}