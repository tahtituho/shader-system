#ifndef MUSIC_H
#define MUSIC_H
#include <string>
#include <fstream>
#include <bass.h>

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
            void seek(double position);

        private:
            HSTREAM stream;
            bool playing;
    };
}
#endif