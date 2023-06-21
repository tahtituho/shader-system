#include "Helpers.h"

std::string DemoSystem::Helpers::readFile(std::string file)
{
    std::ifstream ifs(file);
    std::string content = std::string((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
    return content;
}

int DemoSystem::Helpers::mapWrapToOpenGL(std::string configuration)
{
    if (configuration == "repeat")
    {
        return GL_REPEAT;
    }
    else if(configuration == "mirroredRepeat")
    {
        return GL_MIRRORED_REPEAT;
    }
    else if(configuration == "clampToEdge")
    {
        return GL_CLAMP_TO_EDGE;
    }
    else if (configuration == "clampToBorder")
    {
        return GL_CLAMP_TO_BORDER;
    }
    else if(configuration == "mirrorClampToEdge")
    {
        return GL_MIRROR_CLAMP_TO_EDGE;
    }
    return GL_REPEAT;
}
