#ifndef COSMONAUT_H
#define COSMONAUT_H
#include <sync.h>
#include <string>
#include <math.h>
#define SYNC_PLAYER

namespace DemoSystem {
    class Cosmonaut {
        public:
            Cosmonaut();
            ~Cosmonaut();
            void initialize(double bpm, int rpb);
            bool connectPlayer(std::string host);
            void setFunctions(sync_cb* functions);
            void update(double row);
            double getRowRate();
            void cleanUp();

        private:
            bool player;
            std::string host;
            double BPM;
            int RPB;
            double rowRate;
          
            sync_device* device;
            sync_cb* functions;
            const sync_track *time;
        };
}


#endif