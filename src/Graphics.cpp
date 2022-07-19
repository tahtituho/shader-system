#include "Graphics.h"

void logError(int error, const char *desc)
{
    std::string e = std::to_string(error) + " " + std::string(desc);
    return;
}

void frameBufferResizeCallback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
    //this->postprocessingShader.resizeFBO(width, height);
    // This should have framebuffer and screenSize resize too
}

DemoSystem::Graphics::Graphics() : mainShader(), postprocessingShader()
{
}

DemoSystem::Graphics::~Graphics()
{
}

void DemoSystem::Graphics::initialize(Configuration::Shaders shaders, Configuration::Screen screen, Configuration::Demo demo)
{
    if (!glfwInit())
    {
        return;
    }

    glfwSetErrorCallback(logError);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, shaders.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, shaders.minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (demo.release)
    {
        this->window = glfwCreateWindow(screen.width, screen.height, (demo.group + " - " + demo.name).c_str(), glfwGetPrimaryMonitor(), NULL);
    }
    else
    {
        this->window = glfwCreateWindow(screen.width, screen.height, (demo.group + " - " + demo.name).c_str(), NULL, NULL);
    }

    this->postprocessingShader.setScreenSize(screen.width, screen.height);
    this->mainShader.setScreenSize(screen.width, screen.height);

    if (!this->window)
    {
        glfwTerminate();
        return;
    }

    // Setting window icon
    GLFWimage icons[1];
    GLFWimage image;
    std::vector<unsigned char> imageBuffer;
    unsigned int width;
    unsigned int height;
    // Using temp variables, for some reason casting didn't work as expected
    unsigned error = lodepng::decode(imageBuffer, width, height, demo.icon);
    if (error == 0)
    {
        image.height = height;
        image.width = width;
        image.pixels = &imageBuffer[0];
        icons[0] = image;

        glfwSetWindowIcon(this->window, 1, icons);
    }

    if (demo.release)
    {
        glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    glfwMakeContextCurrent(this->window);

    glfwSwapInterval(1);

    GLenum glewError = glewInit();
    if (GLEW_OK != glewError)
    {
        return;
    }

    glViewport(0, 0, screen.width, screen.height);
    glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);

    return;
}

void DemoSystem::Graphics::registerSynchronizer(DemoSystem::Synchronizer *synchronizer)
{
    this->mainShader.registerSynchronizer(synchronizer);
    this->postprocessingShader.registerSynchronizer(synchronizer);
}

void DemoSystem::Graphics::registerCamera(DemoSystem::Camera *camera)
{
    this->mainShader.registerCamera(camera);
}

void DemoSystem::Graphics::registerTextures(std::list<Textures::Texture> *textures)
{
    this->mainShader.registerTextures(textures);
}

void DemoSystem::Graphics::registerKeyboardCallback(GLFWkeyfun callback)
{
    glfwSetKeyCallback(this->window, callback);
}

void DemoSystem::Graphics::registerMouseMoveCallback(GLFWcursorposfun callback)
{
    glfwSetCursorPosCallback(this->window, callback);
}

void DemoSystem::Graphics::registerMouseButtonsCallback(GLFWmousebuttonfun callback)
{
    glfwSetMouseButtonCallback(this->window, callback);
}

void DemoSystem::Graphics::registerLogger(Logger *logger)
{
    this->logger = logger;
    this->mainShader.registerLogger(logger);
    this->postprocessingShader.registerLogger(logger);
}

void DemoSystem::Graphics::swapBuffers()
{
    glfwSwapBuffers(this->window);
    glfwPollEvents();
}

void DemoSystem::Graphics::requestFullscreen()
{
    if (this->fullscreen)
    {
        glfwRestoreWindow(this->window);
    }
    else
    {
        glfwMaximizeWindow(this->window);
    }
    this->fullscreen = !this->fullscreen;
}

void DemoSystem::Graphics::initStop()
{
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

bool DemoSystem::Graphics::shouldStop()
{
    return glfwWindowShouldClose(this->window);
}

void DemoSystem::Graphics::cleanUp()
{
    mainShader.cleanUp();
    // null check for pp shader?
    postprocessingShader.cleanUp();
    glfwDestroyWindow(this->window);
    glfwTerminate();
}
