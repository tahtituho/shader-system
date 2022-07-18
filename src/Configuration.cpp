#include "Configuration.h"

DemoSystem::Configuration *DemoSystem::Configuration::instance = 0;
DemoSystem::Configuration *DemoSystem::Configuration::getInstance()
{
    if (!instance)
    {
        instance = new Configuration();
    }
    return instance;
}

DemoSystem::Configuration::Configuration()
{
    this->demo.release = false;
    this->demo.name = "demo";
    this->demo.group = "demogroup";
    this->demo.icon = "assets/pics/tt.png";

    this->screen.width = 800;
    this->screen.height = 600;
    this->screen.FPS = 60;

    this->tune.file = "assets/music/tune.mp3";
    this->tune.BPM = 80.0;
    this->tune.frequency = 44000;

    this->sync.enabled = false;
    this->sync.RPB = 8;
    this->sync.host = "localhost";

    this->shaders.vertex = "assets/shaders/vertex.glsl";
    this->shaders.fragment = "assets/shaders/fragment.glsl";
    this->shaders.vertexPost = "assets/shaders/vertex_post.glsl";
    this->shaders.fragmentPost = "assets/shaders/fragment_post.glsl";
    this->shaders.majorVersion = 4;
    this->shaders.minorVersion = 3;
}

bool DemoSystem::Configuration::read(std::string file)
{
    std::ifstream cifs(file);

    if (!cifs.fail())
    {
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

        this->sync.enabled = c["sync"]["enabled"].type() != Json::ValueType::nullValue ? c["sync"]["enabled"].asBool() : this->sync.enabled;
        this->sync.host = c["sync"]["host"].type() != Json::ValueType::nullValue ? c["sync"]["host"].asString() : this->sync.host;
        this->sync.RPB = c["sync"]["RPB"].type() != Json::ValueType::nullValue ? c["sync"]["RPB"].asInt() : this->sync.RPB;

        this->shaders.vertex = c["shaders"]["vertex"].type() != Json::ValueType::nullValue ? c["shaders"]["vertex"].asString() : this->shaders.vertex;
        this->shaders.fragment = c["shaders"]["fragment"].type() != Json::ValueType::nullValue ? c["shaders"]["fragment"].asString() : this->shaders.fragment;
        this->shaders.vertexPost = c["shaders"]["vertexPost"].type() != Json::ValueType::nullValue ? c["shaders"]["vertexPost"].asString() : this->shaders.vertexPost;
        this->shaders.fragmentPost = c["shaders"]["fragmentPost"].type() != Json::ValueType::nullValue ? c["shaders"]["fragmentPost"].asString() : this->shaders.fragmentPost;
        this->shaders.majorVersion = c["shaders"]["majorVersion"].type() != Json::ValueType::nullValue ? c["shaders"]["majorVersion"].asInt() : this->shaders.majorVersion;
        this->shaders.minorVersion = c["shaders"]["minorVersion"].type() != Json::ValueType::nullValue ? c["shaders"]["minorVersion"].asInt() : this->shaders.minorVersion;

        if (c["variables"].type() != Json::ValueType::nullValue)
        {
            for (int i = 0; i < c["variables"].size(); i++)
            {
                Variable t;
                std::string dataType = c["variables"][i]["dataType"].asString();
                if (dataType == "float1")
                {
                    t.type = Variable::DataType::FLOAT1;
                }
                else if (dataType == "float2")
                {
                    t.type = Variable::DataType::FLOAT2;
                }
                else if (dataType == "float3")
                {
                    t.type = Variable::DataType::FLOAT3;
                }
                t.variableName = c["variables"][i]["variable"].asString();
                std::string variableType = c["variables"][i]["variableType"].asString();
                t.trackName = c["variables"][i]["track"].asString();
                this->trackVariables.push_back(t);
            }
        }

        if (c["assets"].type() != Json::ValueType::nullValue)
        {
            for (int i = 0; i < c["assets"].size(); i++)
            {
                Asset a;
                a.name = c["assets"][i]["name"].asString();
                a.file = c["assets"][i]["file"].asString();
                std::string type = c["assets"][i]["type"].asString();
                if (type == "texture")
                {
                    a.type = Configuration::Asset::AssetType::TEXTURE;
                }

                this->assets.push_back(a);
            }
        }
        return true;
    }
    else
    {
        return false;
    }
}

DemoSystem::Configuration::~Configuration()
{
    if (instance)
    {
        delete instance;
    }
}