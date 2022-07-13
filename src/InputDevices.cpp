#include "InputDevices.h"

DemoSystem::InputDevices::InputDevices()
{
}

DemoSystem::InputDevices::~InputDevices()
{
}

void DemoSystem::InputDevices::initialize(Graphics *graphics, Music *music, Logger *logger, Synchronizer *synchronizer, Camera *camera, bool release)
{
    this->graphics = graphics;
    this->music = music;
    this->logger = logger;
    this->synchronizer = synchronizer;
    this->camera = camera;
    this->release = release;
}

void DemoSystem::InputDevices::handleKeyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
    {
        graphics->initStop();
        return;
    }
    if (this->release == false)
    {
        if (action == GLFW_PRESS)
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
                graphics->mainShader.initShaders(vertexSource, fragmentSource);
                std::string vertexPostSource = DemoSystem::Helpers::readFile(configurations->shaders.vertexPost);
                std::string fragmentPostSource = DemoSystem::Helpers::readFile(configurations->shaders.fragmentPost);
                graphics->postprocessingShader.initShaders(vertexPostSource, fragmentPostSource);
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
                DemoSystem::Vector3 cameraPosition = this->camera->getPosition();
                this->logger->write(DemoSystem::Logger::INFO, "position: (" + std::to_string(cameraPosition.x) + ", " + std::to_string(cameraPosition.y) + ", " + std::to_string(cameraPosition.z) + ")");
                DemoSystem::Vector3 viewDir = this->camera->getViewDir();
                this->logger->write(DemoSystem::Logger::INFO, "viewDir: (" + std::to_string(viewDir.x) + ", " + std::to_string(viewDir.y) + ", " + std::to_string(viewDir.z) + ")");
                break;
            }
            }
        }
        if (action == GLFW_PRESS || action == GLFW_REPEAT)
        {
            float factor = (mods & GLFW_MOD_SHIFT) ? 2.0 : 1.0;
            switch (key)
            {
            case GLFW_KEY_W:
            {
                this->camera->moveForward(MOVEMENT_SPEED * factor);
                break;
            }
            case GLFW_KEY_S:
            {
                this->camera->moveBackward(MOVEMENT_SPEED * factor);
                break;
            }
            }
        }
        else if (action == GLFW_RELEASE)
        {
            switch (key)
            {
            case GLFW_KEY_W:
            case GLFW_KEY_S:
            {
                this->camera->zeroVelocity();
                break;
            }
            }
        }
    }
}

void DemoSystem::InputDevices::handleMouseMove(GLFWwindow *window, double x, double y)
{
    if (this->mouseButtonPressed)
    {
        int xDelta = this->previousMouseX - x;
        int yDelta = this->previousMouseY - y;

        this->camera->rotateX(yDelta * ROTATION_SPEED);
        this->camera->rotateY(-xDelta * ROTATION_SPEED);
    }
    this->previousMouseX = x;
    this->previousMouseY = y;
}

void DemoSystem::InputDevices::handleMouseButtons(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
    {
        this->mouseButtonPressed = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        this->mouseButtonPressed = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
