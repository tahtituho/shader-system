#include "InputDevices.h"

DemoSystem::InputDevices::InputDevices()
{
}

DemoSystem::InputDevices::~InputDevices()
{
}

void DemoSystem::InputDevices::initialize(Graphics *graphics, Music *music, Logger *logger, Synchronizer *synchronizer, bool release)
{
    this->graphics = graphics;
    this->music = music;
    this->logger = logger;
    this->synchronizer = synchronizer;
    this->release = release;
}

void DemoSystem::InputDevices::handleKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && this->release == false)
    {
        switch (key)
        {
        case GLFW_KEY_F:
        {
            graphics->requestFullscreen();
            break;
        }
        case GLFW_KEY_R:
        {
            DemoSystem::Configuration *configurations = DemoSystem::Configuration::getInstance();
            std::string vertexSource = DemoSystem::Helpers::readFile(configurations->shaders.vertex);
            std::string fragmentSource = DemoSystem::Helpers::readFile(configurations->shaders.fragment);
            graphics->initShaders(vertexSource, fragmentSource);
            break;
        }
        case GLFW_KEY_M:
        {
            music->silence();
            break;
        }
        case GLFW_KEY_C:
        {
            logger->toggleEnable();
            break;
        }
        case GLFW_KEY_1:
        {
            BasicVariable *user1Variable = &this->synchronizer->basicVariables.at("user");
            user1Variable->value.x == 0.0 ? user1Variable->value.x = 1.0 : user1Variable->value.x = 0.0;
            break;
        }
        case GLFW_KEY_2:
        {
            BasicVariable *user2Variable = &this->synchronizer->basicVariables.at("user");
            user2Variable->value.y == 0.0 ? user2Variable->value.y = 1.0 : user2Variable->value.y = 0.0;
            break;
        }
        case GLFW_KEY_3:
        {
            BasicVariable *user3Variable = &this->synchronizer->basicVariables.at("user");
            user3Variable->value.z == 0.0 ? user3Variable->value.z = 1.0 : user3Variable->value.z = 0.0;
            break;
        }
        case GLFW_KEY_P:
        {
            BasicVariable *positionVariable = &this->synchronizer->basicVariables.at("position");
            this->logger->write(DemoSystem::Logger::INFO, "position: (" + std::to_string(positionVariable->value.x) + ", " + std::to_string(positionVariable->value.y) + ", " + std::to_string(positionVariable->value.z) + ")");
            break;
        }
        }
    }
    if ((action == GLFW_PRESS || action == GLFW_REPEAT) && this->release == false)
    {
        switch (key)
        {
        case GLFW_KEY_D:
        {
            BasicVariable *positionVariable = &this->synchronizer->basicVariables.at("position");
            positionVariable->value.x += MOVEMENT_SPEED;
            break;
        }
        case GLFW_KEY_A:
        {
            BasicVariable *positionVariable = &this->synchronizer->basicVariables.at("position");
            positionVariable->value.x -= MOVEMENT_SPEED;
            break;
        }
        case GLFW_KEY_W:
        {
            BasicVariable *positionVariable = &this->synchronizer->basicVariables.at("position");
            positionVariable->value.z += MOVEMENT_SPEED;
            break;
        }
        case GLFW_KEY_S:
        {
            BasicVariable *positionVariable = &this->synchronizer->basicVariables.at("position");
            positionVariable->value.z -= MOVEMENT_SPEED;
            break;
        }
        }
    }
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
    {
        graphics->initStop();
    }
}

void DemoSystem::InputDevices::handleMouseMove(GLFWwindow *window, double x, double y)
{
    if (this->mouseButtonPressed)
    {
        BasicVariable *mouseVariable = &this->synchronizer->basicVariables.at("mouse");
        mouseVariable->value.x = x;
        mouseVariable->value.y = y;
    }
}

void DemoSystem::InputDevices::handleMouseButtons(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        this->mouseButtonPressed = true;
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        this->mouseButtonPressed = false;
    }
}
