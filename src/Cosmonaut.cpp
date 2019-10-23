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
    if(sync_update(this->device, (int)floor(row * this->getRowRate()), this->functions, &this->rowRate)) {
        if(this->player == true) 
        {
            sync_tcp_connect(this->device, host.c_str(), SYNC_DEFAULT_PORT); 
        }   
    }

    for(std::map<std::string, DemoSystem::Common::Gateway>::iterator it = this->gateways.begin(); it != this->gateways.end(); ++it) {
       if(it->second.type == DemoSystem::Common::Track::FLOAT1) {
           it->second.value.x = sync_get_val(it->second.syncTrack.x, row);
       }
       else if(it->second.type == DemoSystem::Common::Track::FLOAT2) {
           it->second.value.x = sync_get_val(it->second.syncTrack.x, row);
           it->second.value.y = sync_get_val(it->second.syncTrack.y, row);
       }
       else if(it->second.type == DemoSystem::Common::Track::FLOAT3) {
           it->second.value.x = sync_get_val(it->second.syncTrack.x, row);
           it->second.value.y = sync_get_val(it->second.syncTrack.y, row);
           it->second.value.z = sync_get_val(it->second.syncTrack.z, row);
       }
   }
}

void DemoSystem::Cosmonaut::setTracks(std::list<DemoSystem::Common::Track> tracks) {
    for(DemoSystem::Common::Track track : tracks) {
        DemoSystem::Common::Gateway gateway;
        gateway.type = track.type;
        if(track.type == DemoSystem::Common::Track::FLOAT1) {
            gateway.syncTrack.x = sync_get_track(this->device, track.trackName.c_str());  
        }
        else if(track.type == DemoSystem::Common::Track::FLOAT2) {
            gateway.syncTrack.x = sync_get_track(this->device, (track.trackName + ".x").c_str());
            gateway.syncTrack.y = sync_get_track(this->device, (track.trackName + ".y").c_str());
        }
        else if(track.type == DemoSystem::Common::Track::FLOAT3) {
            gateway.syncTrack.x = sync_get_track(this->device, (track.trackName + ".x").c_str());
            gateway.syncTrack.y = sync_get_track(this->device, (track.trackName + ".y").c_str());
            gateway.syncTrack.z = sync_get_track(this->device, (track.trackName + ".z").c_str());
        }
        this->gateways[track.trackName] = gateway;
    }
}

double DemoSystem::Cosmonaut::getRowRate() {
    return this->rowRate;
}

DemoSystem::Common::Gateway DemoSystem::Cosmonaut::getTrack(std::string name) {
    return this->gateways[name];
}

void DemoSystem::Cosmonaut::cleanUp() {
    sync_destroy_device(this->device);
}
