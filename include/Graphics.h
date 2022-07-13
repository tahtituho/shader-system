#ifndef GRAPHICS_H
#define GRAPHICS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <map>
#include "lodepng.h"
#include "Configuration.h"
#include "Textures.h"
#include "Logger.h"
#include "Variable.h"
#include "Camera.h"
#include "Vector3.h"
#include "Shader.h"
#include "Framebuffer.h"

namespace DemoSystem
{
    class Graphics
    {
    public:
        Graphics();
        ~Graphics();
        void initialize(Configuration::Shaders shaders, Configuration::Screen screen, Configuration::Demo demo);
        void addUniforms();

        void registerSynchronizer(DemoSystem::Synchronizer *synchronizer);
        void registerTextures(std::list<Textures::Texture> *textures);
        void registerCamera(DemoSystem::Camera *camera);
        void registerKeyboardCallback(GLFWkeyfun callback);
        void registerMouseMoveCallback(GLFWcursorposfun callback);
        void registerMouseButtonsCallback(GLFWmousebuttonfun callback);
        void registerLogger(Logger *logger);
        
        void swapBuffers();
        void requestFullscreen();
        void initStop();
        bool shouldStop();
        void cleanUp();

        Shader mainShader;
        Framebuffer postprocessingShader;

    protected:
        GLFWwindow *window;
        bool fullscreen;

        Logger *logger;
    };
}
#endif