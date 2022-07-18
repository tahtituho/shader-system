#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Synchronizer.h"
#include "Textures.h"
#include "Logger.h"
#include "Camera.h"

namespace DemoSystem {
    class Shader {
        public:
            Shader();   
            ~Shader();

            void initShaders(std::string vertexSource, std::string fragmentSource);
            void initFrameBuffer();
            void render(double time);
            void renderPost(double time);
            void addUniforms();
            void cleanUp();
            void setScreenSize(int width, int height);

            void registerSynchronizer(DemoSystem::Synchronizer *synchronizer);
            void registerTextures(std::list<Textures::Texture> *textures);
            void registerCamera(DemoSystem::Camera *camera);
            void registerLogger(Logger *logger);

        protected:  
            GLuint program;
            GLuint vertexShader;
            GLuint fragmentShader;
            unsigned int VBO, VAO, EBO;
            int screenWidth, screenHeight;

            void compileShader(const GLenum type, std::string source);

            DemoSystem::Synchronizer *synchronizer;
            std::list<Textures::Texture> *textures;
            DemoSystem::Camera *camera;
            Logger *logger;
    };
}
#endif