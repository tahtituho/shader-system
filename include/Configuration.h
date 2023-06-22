#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#include <string>
#include <json.h>
#include <fstream>
#include <list>
#include "sync.h"

namespace DemoSystem
{
    class Configuration
    {
    public:
        struct Demo
        {
            bool release;
            std::string name;
            std::string group;
            std::string icon;
        };

        struct Screen
        {
            int width;
            int height;
            int FPS;
            int scale;
        };

        struct Tune
        {
            std::string file;
            double BPM;
            int RPB;
            int frequency;
        };

        struct Sync
        {
            bool enabled;
            int RPB;
            std::string host;
        };

        struct Shaders
        {
            std::string vertex;
            std::string fragment;
            std::string vertexPost;
            std::string fragmentPost;
            int majorVersion;
            int minorVersion;
        };

        struct Variable
        {
            enum DataType
            {
                FLOAT1,
                FLOAT2,
                FLOAT3
            };
            enum VariableType
            {
                TRACK_VARIABLE,
                PERSISTENT
            };
            struct Value
            {
                double x;
                double y;
                double z;
            };
            enum ShaderType 
            {
                MAIN,
                POST,
                ALL
            };
            VariableType variableType;
            DataType type;
            Value initialValue;
            ShaderType shaderType;
            std::string trackName;
            std::string variableName;
            bool persistent;
        };

        struct Texture
        {
            std::string file;
            std::string name;
            std::string wrapS;
            std::string wrapT;
        };
        static Configuration *getInstance();
        ~Configuration();
        bool read(std::string file);
        Demo demo;
        Screen screen;
        Tune tune;
        Sync sync;
        Shaders shaders;
        std::list<Configuration::Variable> trackVariables;
        std::list<Configuration::Texture> textures;

    private:
        static Configuration *instance;
        Configuration();
    };

} // namespace DemoSystem

#endif