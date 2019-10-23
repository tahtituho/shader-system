#ifndef COMMON_H
#define COMMON_H
#include <sync.h>
#include <string>
#include <list>

namespace DemoSystem {
    class Common {
        public:
        struct Demo {
            bool release;
            std::string name;
            std::string group;
            std::string icon;
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
            int RPB;
            std::string host;
        };

        struct TrackVariableBond {
            std::string variable;
            std::string track;
        };  

        struct Shaders {
            std::string vertex;
            std::string fragment;
            int majorVersion;
            int minorVersion;
            std::list<TrackVariableBond> trackVariableBonds;
        };

        struct Track {
            enum TrackType {
                FLOAT1,
                FLOAT2,
                FLOAT3
            };
            
            TrackType type;
            std::string trackName;
        };

        struct Asset {
            enum AssetType {
                STATIC_TEXTURE,
                DYNAMIC_TEXTURE
            };

            //Common for all types
            AssetType type;
            std::string file;
            std::string name;

            //For dynamic texture
            std::list<std::string> variables;
            bool once;
            unsigned int width;
            unsigned int height;
        };

        struct SyncTrack {
            const sync_track* x;
            const sync_track* y;
            const sync_track* z;
        };

        struct Vector3 {
            double x;
            double y;
            double z;
        };

        struct Gateway {
            SyncTrack syncTrack;
            Vector3 value;
            Track::TrackType type;
        };
    };
}
#endif