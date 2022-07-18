#ifndef SYNCHRONIZER_H
#define SYNCHRONIZER_H
#include <sync.h>
#include <string>
#include <math.h>
#include <list>
#include <map>
#include <GL/glew.h>
#include "Configuration.h"
#include "Variable.h"
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
        void update(double time);
        void initializeTrackVariables(std::list<Configuration::Variable> trackVariables);
        void initializeBasicVariables();
        void cleanUp();
        std::list<TrackVariable> trackVariables;
        std::map<std::string, BasicVariable> basicVariables;

    private:
        bool player;
        std::string host;
        double BPM;
        int RPB;
        double rowRate;

        sync_device *device;
        sync_cb *functions;

        void addBasicVariable(std::string name, Variable::DataType datatype);
    };
} // namespace DemoSystem

#endif