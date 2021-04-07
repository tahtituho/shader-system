#include "Synchronizer.h"

DemoSystem::Synchronizer::Synchronizer()
{
    this->player = false;
    this->BPM = 120.0;
    this->RPB = 8;
}

DemoSystem::Synchronizer::~Synchronizer()
{
}

void DemoSystem::Synchronizer::initialize(double bpm, int rpb)
{
    this->player = false;
    this->BPM = bpm;
    this->RPB = rpb;
    this->rowRate = this->BPM / 60.0 * this->RPB;

    this->device = sync_create_device("sync");
}

bool DemoSystem::Synchronizer::connectPlayer(std::string host)
{
    this->player = true;
    sync_tcp_connect(this->device, host.c_str(), SYNC_DEFAULT_PORT);
    return true;
}

void DemoSystem::Synchronizer::setFunctions(sync_cb *functions)
{
    this->functions = functions;
}

void DemoSystem::Synchronizer::update(double row)
{
    if (sync_update(this->device, (int)floor(row * this->rowRate), this->functions, &this->rowRate))
    {
        if (this->player == true)
        {
            sync_tcp_connect(this->device, host.c_str(), SYNC_DEFAULT_PORT);
        }
    }

    for (auto it = this->trackVariables.begin(); it != this->trackVariables.end(); it++)
    {
        if (it->type == Variable::DataType::FLOAT1)
        {
            it->value.x = sync_get_val(it->syncTrack.x, row);
        }
        else if (it->type == Variable::DataType::FLOAT2)
        {
            it->value.x = sync_get_val(it->syncTrack.x, row);
            it->value.y = sync_get_val(it->syncTrack.y, row);
        }
        else if (it->type == Variable::DataType::FLOAT3)
        {
            it->value.x = sync_get_val(it->syncTrack.x, row);
            it->value.y = sync_get_val(it->syncTrack.y, row);
            it->value.z = sync_get_val(it->syncTrack.z, row);
        }
    }
}

void DemoSystem::Synchronizer::initializeTrackVariables(std::list<Configuration::Variable> trackVariables)
{
    for (Configuration::Variable track : trackVariables)
    {
        TrackVariable variable;
        switch (track.type)
        {
        case Configuration::Variable::DataType::FLOAT1:
            variable.type = Variable::DataType::FLOAT1;
            variable.syncTrack.x = sync_get_track(this->device, track.trackName.c_str());
            break;
        case Configuration::Variable::DataType::FLOAT2:
            variable.type = Variable::DataType::FLOAT2;
            variable.syncTrack.x = sync_get_track(this->device, (track.trackName + ".x").c_str());
            variable.syncTrack.y = sync_get_track(this->device, (track.trackName + ".y").c_str());
            break;
        case Configuration::Variable::DataType::FLOAT3:
            variable.type = Variable::DataType::FLOAT3;
            variable.syncTrack.x = sync_get_track(this->device, (track.trackName + ".x").c_str());
            variable.syncTrack.y = sync_get_track(this->device, (track.trackName + ".y").c_str());
            variable.syncTrack.z = sync_get_track(this->device, (track.trackName + ".z").c_str());
            break;
        }
        variable.name = track.variableName;
        this->trackVariables.push_back(variable);
    }
}

void DemoSystem::Synchronizer::initializeBasicVariables()
{
    this->addBasicVariable("time", Variable::DataType::FLOAT1);
    this->addBasicVariable("resolution", Variable::DataType::FLOAT2);
    this->addBasicVariable("position", Variable::DataType::FLOAT3);
    this->addBasicVariable("mouse", Variable::DataType::FLOAT2);
    this->addBasicVariable("user", Variable::DataType::FLOAT3);
}

void DemoSystem::Synchronizer::cleanUp()
{
    sync_destroy_device(this->device);
}

void DemoSystem::Synchronizer::addBasicVariable(std::string name, Variable::DataType datatype)
{
    BasicVariable v;
    v.name = name;
    v.type = datatype;
    v.value.x = 0.0;
    v.value.y = 0.0;
    v.value.z = 0.0;
    this->basicVariables[name] = v;
}