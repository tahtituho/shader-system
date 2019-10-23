#ifndef SHADER_H
#define SHADER_H
#include <GL/glew.h>
#include <string>
#include <list>
#include "Logger.h"
#include "Cosmonaut.h"
#include "Common.h"

namespace DemoSystem {
    class Shader {
        public:
            Shader();   
            ~Shader();

           struct UniformVariable {
                GLuint uniform;
                std::string track;
            };

            void initialize(unsigned int width, unsigned int height);
            void initializeUniform(std::string variable, std::string track);
            void initializeUniforms(std::list<DemoSystem::Common::TrackVariableBond> trackVariablesBonds);
            void setSources(std::string vertex, std::string fragment);
            void setCosmonaut(DemoSystem::Cosmonaut* cosmonaut);
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
            std::list<UniformVariable> uniforms;

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

            DemoSystem::Cosmonaut* cosmonaut;
    };
}
#endif