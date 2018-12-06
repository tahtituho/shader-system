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
            int position();
            void seek(int position);

        private:
            HSTREAM stream;
    };
}
#endif