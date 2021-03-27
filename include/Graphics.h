#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <map>
#include "lodepng.h"
#include "Configuration.h"
#include "Synchronizer.h"
#include "Textures.h"
#include "Logger.h"
#include "Variable.h"

namespace DemoSystem
{
    class Graphics
    {
    public:
        Graphics();
        ~Graphics();
        void initialize(Configuration::Shaders shaders, Configuration::Screen screen, Configuration::Demo demo);
        void registerSynchronizer(DemoSystem::Synchronizer *synchronizer);
        void registerTextures(std::list<Textures::Texture> *textures);
        void registerKeyboard(GLFWkeyfun callback);
        void registerMouseMove(GLFWcursorposfun callback);
        void registerMouseButtons(GLFWmousebuttonfun callback);
        void registerLogger(Logger *logger);
        void initShaders(std::string vertexSource, std::string fragmentSource);
        void initFrameBuffer();
        void render(double time);
        void requestFullscreen();
        void initStop();
        bool shouldStop();
        void cleanUp();

    private:
        GLFWwindow *window;
        GLuint program;
        bool fullscreen;

        GLuint vertexShader;
        GLuint fragmentShader;
        unsigned int VBO, VAO, EBO;

        DemoSystem::Synchronizer *synchronizer;
        std::list<Textures::Texture> *textures;
        Logger *logger;
        void compileShader(const GLenum type, std::string source);
    };
}
#endif