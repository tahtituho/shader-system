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

    for (auto it = this->gateways.begin(); it != this->gateways.end(); it++)
    {
        if (it->type == Configuration::Track::TrackType::FLOAT1)
        {
            it->value.x = sync_get_val(it->syncTrack.x, row);
        }
        else if (it->type == Configuration::Track::TrackType::FLOAT2)
        {
            it->value.x = sync_get_val(it->syncTrack.x, row);
            it->value.y = sync_get_val(it->syncTrack.y, row);
        }
        else if (it->type == Configuration::Track::TrackType::FLOAT3)
        {
            it->value.x = sync_get_val(it->syncTrack.x, row);
            it->value.y = sync_get_val(it->syncTrack.y, row);
            it->value.z = sync_get_val(it->syncTrack.z, row);
        }
    }
}

void DemoSystem::Synchronizer::setTracks(std::list<Configuration::Track> tracks)
{
    for (Configuration::Track track : tracks)
    {
        Gateway gateway;
        gateway.type = track.type;
        gateway.name = track.variableName;
        if (track.type == Configuration::Track::FLOAT1)
        {
            gateway.syncTrack.x = sync_get_track(this->device, track.trackName.c_str());
        }
        else if (track.type == Configuration::Track::FLOAT2)
        {
            gateway.syncTrack.x = sync_get_track(this->device, (track.trackName + ".x").c_str());
            gateway.syncTrack.y = sync_get_track(this->device, (track.trackName + ".y").c_str());
        }
        else if (track.type == Configuration::Track::FLOAT3)
        {
            gateway.syncTrack.x = sync_get_track(this->device, (track.trackName + ".x").c_str());
            gateway.syncTrack.y = sync_get_track(this->device, (track.trackName + ".y").c_str());
            gateway.syncTrack.z = sync_get_track(this->device, (track.trackName + ".z").c_str());
        }
        this->gateways.push_back(gateway);
    }
}

void DemoSystem::Synchronizer::cleanUp()
{
    sync_destroy_device(this->device);
}
