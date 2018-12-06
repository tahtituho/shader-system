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
            bool Initialize(int frequency, std::string file);
    };
}
#endif