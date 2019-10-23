#include "Configuration.h"


DemoSystem::Configuration::Configuration() {
    this->demo.release = true;
    this->demo.name = "demo";
    this->demo.group = "demogroup";
    this->demo.icon = "assets/pics/tt.png";

    this->screen.width = 800;
    this->screen.height = 600;
    this->screen.FPS = 60;

    this->tune.file = "tune.mp3";
    this->tune.BPM = 80.0;
    this->tune.frequency = 44000;

    this->sync.RPB = 8;
    this->sync.host = "localhost";

    this->shaders.vertex = "shaders/vertex.glsl";
    this->shaders.fragment = "shaders/fragment.glsl";
    this->shaders.majorVersion = 2;
    this->shaders.minorVersion = 1;
}

bool DemoSystem::Configuration::read(std::string file) {
    std::ifstream cifs(file);

    if(!cifs.fail()) {
        Json::Reader r;
        Json::Value c;
        r.parse(cifs, c);

        this->demo.release = c["demo"]["release"].type() != Json::ValueType::nullValue ? c["demo"]["release"].asBool() : this->demo.release;
        this->demo.name = c["demo"]["name"].type() != Json::ValueType::nullValue ? c["demo"]["name"].asString() : this->demo.name;
        this->demo.group = c["demo"]["group"].type() != Json::ValueType::nullValue ? c["demo"]["group"].asString() : this->demo.group;
        this->demo.icon = c["demo"]["icon"].type() != Json::ValueType::nullValue ? c["demo"]["icon"].asString() : this->demo.icon;

        this->screen.width = c["screen"]["width"].type() != Json::ValueType::nullValue ? c["screen"]["width"].asInt() : this->screen.width;
        this->screen.height = c["screen"]["height"].type() != Json::ValueType::nullValue ? c["screen"]["height"].asInt() : this->screen.height;
        this->screen.FPS = c["screen"]["FPS"].type() != Json::ValueType::nullValue ? c["screen"]["FPS"].asInt() : this->screen.FPS;

        this->tune.file = c["music"]["file"].type() != Json::ValueType::nullValue ? c["music"]["file"].asString() : this->tune.file;
        this->tune.BPM = c["music"]["BPM"].type() != Json::ValueType::nullValue ? c["music"]["BPM"].asDouble() : this->tune.BPM;
        this->tune.frequency = c["music"]["frequency"].type() != Json::ValueType::nullValue ? c["music"]["frequency"].asInt() : this->tune.frequency;

        this->sync.host = c["sync"]["host"].type() != Json::ValueType::nullValue ? c["sync"]["host"].asString() : this->sync.host;
        this->sync.RPB = c["sync"]["RPB"].type() != Json::ValueType::nullValue ? c["sync"]["RPB"].asInt() : this->sync.RPB;

        this->shaders.vertex = c["shaders"]["vertex"].type() != Json::ValueType::nullValue ? c["shaders"]["vertex"].asString() : this->shaders.vertex;
        this->shaders.fragment = c["shaders"]["fragment"].type() != Json::ValueType::nullValue ? c["shaders"]["fragment"].asString() : this->shaders.fragment;
        this->shaders.majorVersion = c["shaders"]["majorVersion"].type() != Json::ValueType::nullValue ? c["shaders"]["majorVersion"].asInt() : this->shaders.majorVersion;
        this->shaders.minorVersion = c["shaders"]["minorVersion"].type() != Json::ValueType::nullValue ? c["shaders"]["minorVersion"].asInt() : this->shaders.minorVersion;
        for(int i = 0; i < c["shaders"]["uniforms"].size(); i++) {
            DemoSystem::Common::TrackVariableBond tvb;
            tvb.track = c["shaders"]["uniforms"][i]["track"].asString();
            tvb.variable = c["shaders"]["uniforms"][i]["variable"].asString();
            this->shaders.trackVariableBonds.push_back(tvb);
        }
        if(c["tracks"].type() != Json::ValueType::nullValue) {
            for(int i = 0; i < c["tracks"].size(); i++) {
                DemoSystem::Common::Track t;
                t.trackName = c["tracks"][i]["track"].asString();
                std::string type = c["tracks"][i]["type"].asString();
                if(type == "float1") {
                    t.type = DemoSystem::Common::Track::FLOAT1;
                }
                else if(type == "float2") {
                    t.type = DemoSystem::Common::Track::FLOAT2;
                }
                else if(type == "float3") {
                    t.type = DemoSystem::Common::Track::FLOAT3;
                }
                this->tracks.push_back(t);
            }
        }

        if(c["assets"].type() != Json::ValueType::nullValue) {
            for(int i = 0; i < c["assets"].size(); i++) {
                DemoSystem::Common::Asset a;
                a.name = c["assets"][i]["name"].asString();
                a.file = c["assets"][i]["file"].asString();
                std::string type = c["assets"][i]["type"].asString();
                if(type == "staticTexture") {
                    a.type = DemoSystem::Common::Asset::AssetType::STATIC_TEXTURE;
                }
                else if(type == "dynamicTexture") {
                    a.type = DemoSystem::Common::Asset::AssetType::DYNAMIC_TEXTURE;
                    a.once = c["assets"][i]["once"].asBool();
                    a.width = c["assets"][i]["width"].asInt();
                    a.height = c["assets"][i]["height"].asInt();
                    if(c["assets"][i]["variables"].type() != Json::ValueType::nullValue) {
                        for(int j = 0; j < c["assets"][i]["variables"].size(); j++) {
                            a.variables.push_back(c["assets"][i]["variables"][j].asString());
                        }
                    }
                }
                
                this->assets.push_back(a);
            }
        }
        return true;
    }
    else {
        return false;
    }
}

DemoSystem::Configuration::~Configuration() {

}