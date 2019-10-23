#ifndef COSMONAUT_H
#define COSMONAUT_H
#include <sync.h>
#include <string>
#include <math.h>
#include <list>
#include <map>
#include <GL/gl.h>
#include "Common.h"
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
            void setTracks(std::list<DemoSystem::Common::Track> tracks);
            DemoSystem::Common::Gateway getTrack(std::string name);
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
            std::map<std::string, DemoSystem::Common::Gateway> gateways;
        };
}


#endif