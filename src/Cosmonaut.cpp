#include "Cosmonaut.h"

DemoSystem::Cosmonaut::Cosmonaut() {
    this->player = false;
    this->BPM = 120.0;
    this->RPB = 8;
}

DemoSystem::Cosmonaut::~Cosmonaut() {

}

void DemoSystem::Cosmonaut::initialize(double bpm, int rpb) {
    this->player = false;
    this->BPM = bpm;
    this->RPB = rpb;
    this->rowRate = this->BPM / 60.0 * this->RPB;

    this->device = sync_create_device("sync");
}

bool DemoSystem::Cosmonaut::connectPlayer(std::string host) {
    this->player = true;
    sync_tcp_connect(this->device, host.c_str(), SYNC_DEFAULT_PORT); 
}

void DemoSystem::Cosmonaut::setFunctions(sync_cb* functions) {
    this->functions = functions;
}

void DemoSystem::Cosmonaut::update(double row) {
    if(sync_update(this->device, (int)floor(row), this->functions, &this->rowRate)) {
        if(this->player == true) 
        {
            sync_tcp_connect(this->device, host.c_str(), SYNC_DEFAULT_PORT); 
        }   
    }

    for(std::list<DemoSystem::Cosmonaut::Gateway>::iterator it = this->gateways.begin(); it != this->gateways.end(); ++it) {
       if(it->type == DemoSystem::Track::FLOAT1) {
           it->value.x = sync_get_val(it->syncTrack.x, row);
       }
       else if(it->type == DemoSystem::Track::FLOAT2) {
           it->value.x = sync_get_val(it->syncTrack.x, row);
           it->value.y = sync_get_val(it->syncTrack.y, row);
       }
       else if(it->type == DemoSystem::Track::FLOAT3) {
           it->value.x = sync_get_val(it->syncTrack.x, row);
           it->value.y = sync_get_val(it->syncTrack.y, row);
           it->value.z = sync_get_val(it->syncTrack.z, row);
       }
   }
}

void DemoSystem::Cosmonaut::setTracks(std::list<DemoSystem::Track> tracks) {
    for(DemoSystem::Track track : tracks) {
        Gateway gateway;
        gateway.type = track.type;
        gateway.name = track.variableName;
        if(track.type == DemoSystem::Track::FLOAT1) {
            gateway.syncTrack.x = sync_get_track(this->device, track.trackName.c_str());  
        }
        else if(track.type == DemoSystem::Track::FLOAT2) {
            gateway.syncTrack.x = sync_get_track(this->device, (track.trackName + ".x").c_str());
            gateway.syncTrack.y = sync_get_track(this->device, (track.trackName + ".y").c_str());
        }
        else if(track.type == DemoSystem::Track::FLOAT3) {
            gateway.syncTrack.x = sync_get_track(this->device, (track.trackName + ".x").c_str());
            gateway.syncTrack.y = sync_get_track(this->device, (track.trackName + ".y").c_str());
            gateway.syncTrack.z = sync_get_track(this->device, (track.trackName + ".z").c_str());
        }
        this->gateways.push_back(gateway);
    }
}

double DemoSystem::Cosmonaut::getRowRate() {
    return this->rowRate;
}

void DemoSystem::Cosmonaut::cleanUp() {
    sync_destroy_device(this->device);
}
