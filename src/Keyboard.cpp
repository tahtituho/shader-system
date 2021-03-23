#include "Keyboard.h"

DemoSystem::Keyboard::Keyboard()
{
}

DemoSystem::Keyboard::~Keyboard()
{
}

void DemoSystem::Keyboard::initialize(Graphics graphics, Music music, Logger logger, bool release)
{
    this->graphics = graphics;
    this->music = music;
    this->logger = logger;
    this->release = release;
}

void DemoSystem::Keyboard::handleKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_F:
            if (this->release == false)
            {
                graphics.requestFullscreen();
            }
            break;
        case GLFW_KEY_R:
            if (this->release == false)
            {
                DemoSystem::Configuration *configurations = DemoSystem::Configuration::getInstance();
                std::string vertexSource = DemoSystem::Helpers::readFile(configurations->shaders.vertex);
                std::string fragmentSource = DemoSystem::Helpers::readFile(configurations->shaders.fragment);
                graphics.initShaders(vertexSource, fragmentSource);
            }
            break;
        case GLFW_KEY_S:
            if (this->release == false)
            {
                music.silence();
            }
            break;
        case GLFW_KEY_C:
            if (this->release == false)
            {
                logger.toggleEnable();
            }
            break;
        case GLFW_KEY_ESCAPE:
            graphics.initStop();
            break;
        }
    }
}
