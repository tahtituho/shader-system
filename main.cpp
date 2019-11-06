#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "lodepng.h"
#include "Configuration.h"
#include "Music.h"
#include "Cosmonaut.h"
#include "Shader.h"
#include "Logger.h"
#include "Helpers.h"
#include "TextureManager.h"

const char* VERSION = "1.4";
#define SYNC_PLAYER

void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
void mainLoop();
bool initTextureShaders(bool first);
void logError(int error, const char* desc);
void cleanUp();
void writeToLogFile( const std::string &text, bool clean);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void musicPause(void* c, int flag);
void musicSetRow(void* c, int row);
int musicPlaying(void* c);

bool fullscreen;

GLFWwindow* window;

DemoSystem::Configuration configurations;
DemoSystem::Music music;
DemoSystem::Cosmonaut cosmonaut;
DemoSystem::Shader shader;
DemoSystem::Logger logger;
DemoSystem::TextureManager textureManager;

int main(int argc, char* args[])
{
    
    std::string confFile = "configuration.json";
    if(argc > 1) {
        confFile = std::string(args[1]);
    }
    
    configurations.read(confFile);

    if(configurations.shaders.vertex.empty() || configurations.shaders.fragment.empty())
    {
        std::cout << "provide vertex and fragment shaders in configuration file";
        return 1;
    }

    if (!glfwInit()) {
        std::cout << "glfwInit failed";
        return -1;
    }

    glfwSetErrorCallback(logError);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, configurations.shaders.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, configurations.shaders.minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (configurations.demo.release) {
        // Two ways to start as fullscreen, need to check which is more suitable
        // This is true fullscreen!
        window = glfwCreateWindow(configurations.screen.width, configurations.screen.height, (configurations.demo.group + " : " + configurations.demo.name).c_str(), glfwGetPrimaryMonitor(), NULL);
        // This is windowed fullscreen!
        /*const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
        window = glfwCreateWindow(mode->width, mode->height, (configurations.demo.group + " : " + configurations.demo.name).c_str(), glfwGetPrimaryMonitor(), NULL);*/
    }
    else {
        window = glfwCreateWindow(configurations.screen.width, configurations.screen.height, (configurations.demo.group + " : " + configurations.demo.name).c_str(), NULL, NULL);
    }

    if(!window) {
        std::cout << "window creation failed";
        glfwTerminate();
        return -1;
    }

    // Setting window icon
    GLFWimage icons[1];
    unsigned error;
    GLFWimage image;
    std::vector<unsigned char> imageBuffer;
    unsigned int width;
    unsigned int height;
    // Using temp variables, for some reason casting didn't work as expected
    error = lodepng::decode(imageBuffer, width, height, configurations.demo.icon);
    if (error == 0) {
        image.height = height;
        image.width = width;
        image.pixels = &imageBuffer[0];
        icons[0] = image;

        glfwSetWindowIcon(window, 1, icons);
    }

    if (configurations.demo.release) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    glfwSetKeyCallback(window, handleKeyboard);
    glfwMakeContextCurrent(window);
    
    glfwSwapInterval(1);

    GLenum glewError = glewInit();
    if (GLEW_OK != glewError)
    {
        std::cout << "glew error: " << std::string((const char*)glewGetErrorString(glewError));
        return false;
    }
    
    glViewport(0, 0, configurations.screen.width, configurations.screen.height);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    
    logger.initialize(25, 0.0, configurations.screen.height, !configurations.demo.release);

    logger.write(DemoSystem::Logger::INFO, "shader system version "  + std::string(VERSION) + " by tähtituho 2019");
    logger.write(DemoSystem::Logger::INFO, "opengl vendor:   " + std::string((const char*)glGetString(GL_VENDOR)));
    logger.write(DemoSystem::Logger::INFO, "opengl renderer  " + std::string((const char*)glGetString(GL_RENDERER)));
    logger.write(DemoSystem::Logger::INFO, "opengl version:  " + std::string((const char*)glGetString(GL_VERSION)));
    logger.write(DemoSystem::Logger::INFO, "shading version: " + std::string((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION)));
    logger.write(DemoSystem::Logger::INFO, "context version  " + std::to_string(configurations.shaders.majorVersion) + "." + std::to_string(configurations.shaders.minorVersion));
    logger.write(DemoSystem::Logger::INFO, "bass version:    " + std::to_string(BASS_GetVersion()));

    textureManager.setTextures(configurations.assets);
    textureManager.initializeTextures();
    
    music.initialize(configurations.tune.frequency, configurations.tune.file);
    cosmonaut.initialize(configurations.tune.BPM, configurations.sync.RPB);
    if (!configurations.demo.release) {
        cosmonaut.connectPlayer(configurations.sync.host);
    }
    sync_cb functions;
    functions.is_playing = &musicPlaying;
    functions.pause = &musicPause;
    functions.set_row = &musicSetRow;
    cosmonaut.setFunctions(&functions);
    cosmonaut.setTracks(configurations.tracks);

    shader.initialize(configurations.screen.width, configurations.screen.height);
    shader.setSources(DemoSystem::Helpers::readFile(configurations.shaders.vertex), DemoSystem::Helpers::readFile(configurations.shaders.fragment));
    shader.initializeTextures(configurations.shaders.textures);
    shader.initializeUniforms(configurations.shaders.trackVariableBonds);
    shader.setCosmonaut(&cosmonaut);
    shader.setTextureManager(&textureManager);
    DemoSystem::Logger::Message m = shader.initShader();
    if(m.failure) {
        logger.write(DemoSystem::Logger::ERR, m.content);
    }
    music.play();
    mainLoop();
    cleanUp();
    return 0;
}

void musicPause(void* rr, int flag) {
    if(flag == 1) {
        music.pause();
    }
    else {
        music.play();
    }
}

void musicSetRow(void* rr, int row) {
    double rowRate = *((double *)rr);
    music.seek(row / rowRate);
}

int musicPlaying(void* rr) {
    if(music.isPlaying()) {
        return 1;
    }
    else {
        return 0;
    }
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}  

void handleKeyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS) {
        switch(key)
        {
            case GLFW_KEY_F:
                if(configurations.demo.release == false) {
                    if(fullscreen) {
                        glfwRestoreWindow(window);
                    }
                    else {
                        glfwMaximizeWindow(window);
                    }
                    fullscreen = !fullscreen;
                }
                break;
            case GLFW_KEY_R:
                if(configurations.demo.release == false) {
                    shader.cleanShader();
                    shader.setSources(DemoSystem::Helpers::readFile(configurations.shaders.vertex), DemoSystem::Helpers::readFile(configurations.shaders.fragment));
                    DemoSystem::Logger::Message m = shader.initShader();
                    if(m.failure) {
                        logger.write(DemoSystem::Logger::ERR, m.content);
                    }
                }
                break;
            case GLFW_KEY_S:
                if(configurations.demo.release == false) {
                    music.silence();
                }          
                break;
            case GLFW_KEY_C:
                if(configurations.demo.release == false) {
                    logger.toggleEnable();
                }          
                break;
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
        }
    }
}

void mainLoop()
{
    while(!glfwWindowShouldClose(window)) {
        double time = music.position();
        cosmonaut.update(time);
        shader.render(time);
        logger.render();
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(configurations.demo.release == true && music.hasMusicEnded() == true) {
           glfwSetWindowShouldClose(window, GLFW_TRUE); 
        }
    }
}

void logError(int error, const char* desc)
{
    std::cout << "glfw error: " << std::to_string(error) << " " << std::string(desc);
}

void cleanUp()
{
    glfwDestroyWindow(window);
    cosmonaut.cleanUp();
    music.cleanUp();
    glfwTerminate();
}