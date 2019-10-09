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
            Shader(std::string vertexFile, std::string fragmentFile, unsigned int width, unsigned int height);    
            ~Shader();
            struct Uniform {
                std::string name;
                GLint handle;
            };

            void initialize();
           
            DemoSystem::Logger::Message initShader();
            DemoSystem::Logger::Message compileShader(GLenum type);
            void render(double time);

        private:
            std::string readSource(std::string file);

            unsigned int width;
            unsigned int height;

            std::string vertexFile;
            GLuint vertex;

            std::string fragmentFile;
            GLuint fragment;

            GLuint program;
            std::map<std::string, DemoSystem::Shader::Uniform> uniforms;

            float vertices[12] = {
                1.0f,  1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                -1.0f, -1.0f, 0.0f,
                -1.0f,  1.0f, 0.0f
            };

            unsigned int indices[6] = {
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
            };

            unsigned int VBO, VAO, EBO;

            GLint timeUniform;
            GLint resolutionUniform;
    };
}
#endif