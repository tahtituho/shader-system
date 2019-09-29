#ifndef MUSIC_H
#define MUSIC_H
#include <string>
#include <fstream>
#include <bass.h>
#include "Cosmonaut.h"

namespace DemoSystem {
    class Music {
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
            void cleanUp();

        private:
            HSTREAM stream;
            bool playing;
            bool silent;
    };
}
#endif