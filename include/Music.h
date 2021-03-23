#ifndef MUSIC_H
#define MUSIC_H
#include <string>
#include <fstream>
#include <bass.h>
#include "Synchronizer.h"

namespace DemoSystem
{
    class Music
    {
    public:
        Music();
        ~Music();
        bool initialize(int frequency, std::string file);
        void play();
        void pause();
        bool isPlaying();
        double position();
        void seek(double row);
        void silence();
        bool hasMusicEnded();
        std::string version();
        void cleanUp();

    private:
        HSTREAM stream;
        bool playing;
        bool silent;
        bool hasEnded;
        void static CALLBACK musicEndCallback(HSYNC handle, DWORD channel, DWORD data, void *user);
    };
} // namespace DemoSystem
#endif