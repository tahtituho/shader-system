#include "ShaderSystemConfig.h"
#include "Graphics.h"
#include "Configuration.h"
#include "Music.h"
#include "Synchronizer.h"
#include "Textures.h"
#include "Logger.h"
#include "InputDevices.h"
#include "Helpers.h"
#include "Camera.h"

const std::string VERSION = ShaderSystem_VERSION_MAJOR + "." + ShaderSystem_VERSION_MINOR;
void mainLoop();
void cleanUp();

void handleKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void handleMouseMove(GLFWwindow *window, double x, double y);
void handleMouseButtons(GLFWwindow *window, int button, int action, int mods);

void musicPause(void *c, int flag);
void musicSetRow(void *c, int row);
int musicPlaying(void *c);

DemoSystem::Configuration *configurations;
DemoSystem::Music music;
DemoSystem::Synchronizer synchronizer;
DemoSystem::Textures textures;
DemoSystem::Logger logger;
DemoSystem::Graphics graphics;
DemoSystem::InputDevices inputDevices;
DemoSystem::Camera camera;

int main(int argc, char *args[])
{
    std::string confFile = "configuration.json";
    if (argc > 1)
    {
        confFile = std::string(args[1]);
    }
    configurations = DemoSystem::Configuration::getInstance();
    configurations->read(confFile);

    graphics.initialize(configurations->shaders, configurations->screen, configurations->demo);
    logger.initialize(25, 0.0, configurations->screen.height, !configurations->demo.release);
    std::ostringstream stream;
    stream << "shader system version " << ShaderSystem_VERSION_MAJOR << "." << ShaderSystem_VERSION_MINOR << " by tahtituho 2019";
    logger.write(DemoSystem::Logger::INFO, stream.str());
    logger.write(DemoSystem::Logger::INFO, "opengl vendor:   " + std::string((const char *)glGetString(GL_VENDOR)));
    logger.write(DemoSystem::Logger::INFO, "opengl renderer  " + std::string((const char *)glGetString(GL_RENDERER)));
    logger.write(DemoSystem::Logger::INFO, "opengl version:  " + std::string((const char *)glGetString(GL_VERSION)));
    logger.write(DemoSystem::Logger::INFO, "shading version: " + std::string((const char *)glGetString(GL_SHADING_LANGUAGE_VERSION)));
    logger.write(DemoSystem::Logger::INFO, "context version: " + std::to_string(configurations->shaders.majorVersion) + "." + std::to_string(configurations->shaders.minorVersion));
    logger.write(DemoSystem::Logger::INFO, "bass version:    " + music.version());

    music.initialize(configurations->tune.frequency, configurations->tune.file);
    synchronizer.initialize(configurations->tune.BPM, configurations->sync.RPB);

    if (!configurations->demo.release)
    {
        synchronizer.connectPlayer(configurations->sync.host);
    }
    sync_cb functions;
    functions.is_playing = &musicPlaying;
    functions.pause = &musicPause;
    functions.set_row = &musicSetRow;
    synchronizer.setFunctions(&functions);
    synchronizer.initializeTrackVariables(configurations->trackVariables);
    synchronizer.initializeBasicVariables();

    textures.setTextures(configurations->assets);

    graphics.registerLogger(&logger);
    graphics.registerKeyboardCallback(&handleKeyboard);
    graphics.registerMouseMoveCallback(&handleMouseMove);
    graphics.registerMouseButtonsCallback(&handleMouseButtons);
    graphics.registerTextures(&textures.textures);
    graphics.registerSynchronizer(&synchronizer);
    graphics.registerCamera(&camera);
    std::string vertexSource = DemoSystem::Helpers::readFile(configurations->shaders.vertex);
    std::string fragmentSource = DemoSystem::Helpers::readFile(configurations->shaders.fragment);
    graphics.initShaders(vertexSource, fragmentSource);
    graphics.initFrameBuffer();

    inputDevices.initialize(&graphics, &music, &logger, &synchronizer, &camera, configurations->demo.release);

    music.play();
    mainLoop();
    cleanUp();
    return 0;
}

void musicPause(void *rr, int flag)
{
    if (flag == 1)
    {
        music.pause();
    }
    else
    {
        music.play();
    }
}

void musicSetRow(void *rr, int row)
{
    double rowRate = *((double *)rr);
    music.seek(row / rowRate);
}

int musicPlaying(void *rr)
{
    if (music.isPlaying())
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void mainLoop()
{
    while (!graphics.shouldStop())
    {
        double position = music.position();
        synchronizer.update(position);
        graphics.render(position);
        camera.update();
        logger.render();

        if (configurations->demo.release == true && music.hasMusicEnded() == true)
        {
            graphics.initStop();
        }
    }
}

void handleKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    inputDevices.handleKeyboard(window, key, scancode, action, mods);
}

void handleMouseMove(GLFWwindow *window, double x, double y)
{
    inputDevices.handleMouseMove(window, x, y);
}

void handleMouseButtons(GLFWwindow *window, int button, int action, int mods)
{
    inputDevices.handleMouseButtons(window, button, action, mods);
}

void cleanUp()
{
    logger.cleanUp();
    synchronizer.cleanUp();
    music.cleanUp();
    graphics.cleanUp();
}
