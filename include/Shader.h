#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <string>
#include <map> 
#include "Logger.h"

namespace DemoSystem {
    class Shader {
        public:
            Shader();   
            ~Shader();
            struct Uniform {
                std::string name;
                GLint handle;
            };

            void initialize(unsigned int width, unsigned int height);
            void setSources(std::string vertec, std::string fragment);
            DemoSystem::Logger::Message initShader();
            DemoSystem::Logger::Message compileShader(GLenum type);
            void render(double time);

        private:
            std::string readSource(std::string file);

            unsigned int width;
            unsigned int height;

            std::string vertexSource;
            GLuint vertex;

            std::string fragmentSource;
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