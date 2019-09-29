#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <map> 
#include "Logger.h"

namespace DemoSystem {
    class Shader {
        public:
            Shader();
            Shader(std::string vertexFile, std::string fragmentFile);    
            ~Shader();
            struct Uniform {
                std::string name;
                GLint handle;
            };

            std::string readSource(std::string file);
            DemoSystem::Logger::Message initShader();
            DemoSystem::Logger::Message compileShader(GLenum type);

        private:
            std::string vertexFile;
            GLuint vertex;

            std::string fragmentFile;
            GLuint fragment;

            GLuint program;
            std::map<std::string, DemoSystem::Shader::Uniform> uniforms;

    };
}
#endif