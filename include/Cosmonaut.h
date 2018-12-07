#ifndef COSMONAUT_H
#define COSMONAUT_H
#include <sync.h>
#include <string>

namespace DemoSystem {
    class Cosmonaut {
        public:
            Cosmonaut();
            ~Cosmonaut();
            void initialize(double bpm, int rpb);
            bool connectPlayer(std::string host);

        private:
            bool player;
            std::string host;
            double BPM;
            int RPB;
            double rowRate;

            sync_device* device;
        };



}

#endif