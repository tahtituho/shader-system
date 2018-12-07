#include "Cosmonaut.h"

DemoSystem::Cosmonaut::Cosmonaut() {
    this->player = false;
    this->BPM = 120.0;
    this->RPB = 8;
}

DemoSystem::Cosmonaut::~Cosmonaut() {
    sync_destroy_device(this->device);
}

void DemoSystem::Cosmonaut::initialize(double bpm, int rpb) {
    this->player = player;
    this->BPM = bpm;
    this->RPB = rpb;
    this->rowRate = this->BPM / 60.0 * this->RPB;

    this->device = sync_create_device("sync");
}

bool DemoSystem::Cosmonaut::connectPlayer(std::string host) {
    this->player = true;
    sync_tcp_connect(this->device, host.c_str(), SYNC_DEFAULT_PORT); 
}