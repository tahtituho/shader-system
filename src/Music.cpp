#include "Music.h"

DemoSystem::Music::Music()
{
    this->playing = false;
    this->hasEnded = false;
    this->silent = false;
}

DemoSystem::Music::~Music()
{
}

bool DemoSystem::Music::initialize(int frequency, std::string file)
{
    if (!BASS_Init(-1, frequency, BASS_DEVICE_STEREO, 0, NULL))
    {
        return false;
    }
    this->stream = BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, BASS_STREAM_PRESCAN);
    if (this->stream == 0)
    {
        int error = BASS_ErrorGetCode();
        return false;
    }

    BASS_ChannelSetSync(this->stream, BASS_SYNC_END, 0, &this->musicEndCallback, this);

    return true;
}

void DemoSystem::Music::play()
{
    if (this->playing == false)
    {
        BASS_ChannelPlay(this->stream, false);
    }
    this->playing = true;
}

void DemoSystem::Music::pause()
{
    if (this->playing == true)
    {
        BASS_ChannelPause(this->stream);
        this->playing = false;
    }
}

bool DemoSystem::Music::isPlaying()
{
    if (this->playing)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

double DemoSystem::Music::position()
{
    QWORD bytePosition = BASS_ChannelGetPosition(this->stream, BASS_POS_BYTE);
    return BASS_ChannelBytes2Seconds(this->stream, bytePosition);
}

void DemoSystem::Music::seek(double row)
{
    QWORD bytePosition = BASS_ChannelSeconds2Bytes(this->stream, row);
    BASS_ChannelSetPosition(this->stream, bytePosition, BASS_POS_BYTE);
}

void CALLBACK DemoSystem::Music::musicEndCallback(HSYNC handle, DWORD channel, DWORD data, void *music)
{
    static_cast<DemoSystem::Music *>(music)->hasEnded = true;
}

bool DemoSystem::Music::hasMusicEnded()
{
    return this->hasEnded;
}

void DemoSystem::Music::silence()
{
    BASS_ChannelSetAttribute(this->stream, BASS_ATTRIB_VOL, this->silent ? 1.0 : 0.0);
    this->silent = !this->silent;
}

std::string DemoSystem::Music::version()
{
    DWORD bassVersion = BASS_GetVersion();
    int version4 = (bassVersion >> 0) & 0xFF;
    int version3 = (bassVersion >> 8) & 0xFF;
    int version2 = (bassVersion >> 16) & 0xFF;
    int version1 = (bassVersion >> 24) & 0xFF;

    std::stringstream stream;
    stream << std::to_string(version1) << "." << std::to_string(version2) << "." << std::to_string(version3) << "." << std::to_string(version4);
    std::string result(stream.str());
    return result;
}

void DemoSystem::Music::cleanUp()
{
    BASS_SampleFree(this->stream);
    BASS_Free();
}