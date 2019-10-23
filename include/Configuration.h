#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <string>
#include <json.h>
#include <fstream>
#include <list>
#include "sync.h"
#include "Common.h"
namespace DemoSystem  {
    class Configuration {
        public:
            Configuration();
            ~Configuration();
            bool read(std::string file);
            DemoSystem::Common::Demo demo;
            DemoSystem::Common::Screen screen;
            DemoSystem::Common::Tune tune;   
            DemoSystem::Common::Sync sync;
            DemoSystem::Common::Shaders shaders;
            std::list<DemoSystem::Common::Track> tracks;
            std::list<DemoSystem::Common::Asset> assets;
    };

}

#endif