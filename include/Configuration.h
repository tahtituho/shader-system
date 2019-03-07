#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <string>
#include <json.h>
#include <fstream>
#include <list>
#include "sync.h"

namespace DemoSystem  {
    struct Demo {
        std::string name;
        std::string group;
    };

    struct Screen {
        int width;
        int height;
        int FPS;
    };

    struct Tune {
        std::string file;
        double BPM;
        int RPB;
        int frequency;
    };

    struct Sync {
        std::string file;
        int RPB;
        std::string host;
    };

    struct Shaders {
        std::string vertex;
        std::string fragment;
        int majorVersion;
        int minorVersion;
    };

    struct Track {
        enum TrackType {
            FLOAT1,
            FLOAT2,
            FLOAT3
        };
        
        TrackType type;
        std::string trackName;
        std::string variableName;
    };

    struct Asset {
        enum AssetType {
            TEXTURE
        };

        AssetType type;
        std::string file;
        std::string name;
    };

    class Configuration {
        public:
            Configuration();
            ~Configuration();
            bool read(std::string file);
            Demo demo;
            Screen screen;
            Tune tune;   
            Sync sync;
            Shaders shaders;
            std::list<DemoSystem::Track> tracks;
            std::list<DemoSystem::Asset> assets;
    };

}

#endif