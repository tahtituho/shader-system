#ifndef COSMONAUT_H
#define COSMONAUT_H
#include <sync.h>
#include <string>
#include <math.h>
#include <list>
#include <GL/gl.h>
#include "Configuration.h"
#define SYNC_PLAYER

namespace DemoSystem
{
    class Synchronizer
    {
    public:
        Synchronizer();
        ~Synchronizer();
        void initialize(double bpm, int rpb);
        bool connectPlayer(std::string host);
        void setFunctions(sync_cb *functions);
        void update(double row);
        void setTracks(std::list<Configuration::Track> tracks);
        void cleanUp();

        struct SyncTrack
        {
            const sync_track *x;
            const sync_track *y;
            const sync_track *z;
        };

        struct Vector3
        {
            double x;
            double y;
            double z;
        };

        struct Gateway
        {
            SyncTrack syncTrack;
            Vector3 value;
            Configuration::Track::TrackType type;
            std::string name;
            GLint uniform;
        };

        std::list<Gateway> gateways;

    private:
        bool player;
        std::string host;
        double BPM;
        int RPB;
        double rowRate;

        sync_device *device;
        sync_cb *functions;
    };
} // namespace DemoSystem

#endif