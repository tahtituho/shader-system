#ifndef HELPERS_H
#define HELPERS_H
#include <string>
#include <fstream>
#include <map>
#include <GL/glew.h>

namespace DemoSystem
{
    class Helpers
    {
    public:
        static std::string readFile(std::string file);
        static int mapWrapToOpenGL(std::string configuration);
    };
} // namespace DemoSystem
#endif