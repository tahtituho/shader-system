#include "Configuration.h"


DemoSystem::Configuration::Configuration() {
    this->demo.name = "demo";
    this->demo.group = "demogroup";

    this->screen.width = 800;
    this->screen.height = 600;
    this->screen.FPS = 60;

    this->tune.file = "tune.mp3";
    this->tune.BPM = 80.0;
    this->tune.RPB = 8;
    this->tune.frequency = 44000;

    this->shaders.vertex = "shaders/vertex.glsl";
    this->shaders.fragment = "shaders/fragment.glsl";
}

bool DemoSystem::Configuration::read(std::string file) {
    std::ifstream cifs(file);

    if(!cifs.fail()) {
        Json::Reader r;
        Json::Value c;
        r.parse(cifs, c);

        this->demo.name = c["demo"]["name"].type() != Json::ValueType::nullValue ? c["demo"]["name"].asString() : this->demo.name;
        this->demo.group = c["demo"]["group"].type() != Json::ValueType::nullValue ? c["demo"]["group"].asString() : this->demo.group;

        this->screen.width = c["screen"]["width"].type() != Json::ValueType::nullValue ? c["screen"]["width"].asInt() : this->screen.width;
        this->screen.height = c["screen"]["height"].type() != Json::ValueType::nullValue ? c["screen"]["height"].asInt() : this->screen.height;
        this->screen.FPS = c["screen"]["FPS"].type() != Json::ValueType::nullValue ? c["screen"]["FPS"].asInt() : this->screen.FPS;

        this->tune.file = c["music"]["file"].type() != Json::ValueType::nullValue ? c["music"]["file"].asString() : this->tune.file;
        this->tune.BPM = c["music"]["BPM"].type() != Json::ValueType::nullValue ? c["music"]["BPM"].asDouble() : this->tune.BPM;
        this->tune.RPB = c["music"]["RPB"].type() != Json::ValueType::nullValue ? c["music"]["RPB"].asInt() : this->tune.RPB;
        this->tune.frequency = c["music"]["frequency"].type() != Json::ValueType::nullValue ? c["music"]["frequency"].asInt() : this->tune.frequency;

        this->shaders.vertex = c["shaders"]["vertex"].type() != Json::ValueType::nullValue ? c["shaders"]["vertex"].asString() : this->shaders.vertex;
        this->shaders.fragment = c["shaders"]["fragment"].type() != Json::ValueType::nullValue ? c["shaders"]["fragment"].asString() : this->shaders.fragment;
        return true;
    }
    else {
        return false;
    }
}

DemoSystem::Configuration::~Configuration() {

}