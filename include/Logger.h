#ifndef LOGGER_H
#define LOGGER_H
#include <string>
#include <sstream>
#include <list>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLT_IMPLEMENTATION
#include "gltext.h"

namespace DemoSystem
{
    class Logger
    {
    public:
        Logger();
        ~Logger();

        enum LOG_LEVEL
        {
            INFO,
            ERR
        };
        void initialize(int size, float x, float y, bool e);
        void write(LOG_LEVEL level, std::string text);
        void render();
        bool isEnabled();
        void toggleEnable();
        void cleanUp();

    private:
        std::list<GLTtext *> buffer;
        int size;
        float x;
        float y;
        bool enabled;
    };
} // namespace DemoSystem
#endif